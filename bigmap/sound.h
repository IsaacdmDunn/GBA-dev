// === Music classes ========================================================
/*
made by ian finlayson
https://ianfinlayson.net/
modified to work with libtonc
*/
/* define the timer control registers */
#include <tonc.h>
#include "Music/zelda_treasure_16K_mono.h"


bool resetMusic = true;

volatile unsigned short* timer0_data = (volatile unsigned short*)0x4000100;
volatile unsigned short* timer0_control = (volatile unsigned short*)0x4000102;
// /* pointers to the DMA source/dest locations and control registers */
volatile unsigned int* dma1_source = (volatile unsigned int*)0x40000BC;
volatile unsigned int* dma1_destination = (volatile unsigned int*)0x40000C0;
volatile unsigned int* dma1_control = (volatile unsigned int*)0x40000C4;
volatile unsigned int* dma2_source = (volatile unsigned int*)0x40000C8;
volatile unsigned int* dma2_destination = (volatile unsigned int*)0x40000CC;
volatile unsigned int* dma2_control = (volatile unsigned int*)0x40000D0;
volatile unsigned int* dma3_source = (volatile unsigned int*)0x40000D4;
volatile unsigned int* dma3_destination = (volatile unsigned int*)0x40000D8;
volatile unsigned int* dma3_control = (volatile unsigned int*)0x40000DC;
/* copy data using DMA channel 3 (normal memory transfers) */
void memcpy16_dma(unsigned short* dest, unsigned short* source, int amount) {
	*dma3_source = (unsigned int)source;
	*dma3_destination = (unsigned int)dest;
	*dma3_control = DMA_ENABLE | 0x00000000 | amount;
}
/* allows turning on and off sound for the GBA altogether */
volatile unsigned short* master_sound = (volatile unsigned short*)0x4000084;
/* has various bits for controlling the direct sound channels */
volatile unsigned short* sound_control = (volatile unsigned short*)0x4000082;
/* the location of where sound samples are placed for each channel */
volatile unsigned char* fifo_buffer_a = (volatile unsigned char*)0x40000A0;
volatile unsigned char* fifo_buffer_b = (volatile unsigned char*)0x40000A4;
/* global variables to keep track of how much longer the sounds are to play */
unsigned int channel_a_vblanks_remaining = 0;
unsigned int channel_a_total_vblanks = 0;
unsigned int channel_b_vblanks_remaining = 0;
/* the global interrupt enable register */
volatile unsigned short* interrupt_enable = (unsigned short*)0x4000208;
/* this register stores the individual interrupts we want */
volatile unsigned short* interrupt_selection = (unsigned short*)0x4000200;
/* this registers stores which interrupts if any occured */
volatile unsigned short* interrupt_state = (unsigned short*)0x4000202;
/* the address of the function to call when an interrupt occurs */
volatile unsigned int* interrupt_callback = (unsigned int*)0x3007FFC;
/* this register needs a bit set to tell the hardware to send the vblank interrupt */
volatile unsigned short* display_interrupts = (unsigned short*)0x4000004;
/* play a sound with a number of samples, and sample rate on one channel 'A' or 'B' */
void play_sound(const signed char* sound, int total_samples, int sample_rate, char channel) {
	/* start by disabling the timer and dma controller (to reset a previous sound) */
	*timer0_control = 0;
	if (channel == 'A') {
		*dma1_control = 0;
	}
	else if (channel == 'B') {
		*dma2_control = 0;
	}

	/* output to both sides and reset the FIFO */
	if (channel == 'A') {
		*sound_control |= SOUND_A_RIGHT_CHANNEL | SOUND_A_LEFT_CHANNEL | SOUND_A_FIFO_RESET;
	}
	else if (channel == 'B') {
		*sound_control |= SOUND_B_RIGHT_CHANNEL | SOUND_B_LEFT_CHANNEL | SOUND_B_FIFO_RESET;
	}

	/* enable all sound */
	*master_sound = SOUND_MASTER_ENABLE;

	/* set the dma channel to transfer from the sound array to the sound buffer */
	if (channel == 'A') {
		*dma1_source = (unsigned int)sound;
		*dma1_destination = (unsigned int)fifo_buffer_a;
		*dma1_control = 0x400000 | DMA_REPEAT | DMA_32 | DMA_SYNC_TO_TIMER | DMA_ENABLE;
	}
	else if (channel == 'B') {
		*dma2_source = (unsigned int)sound;
		*dma2_destination = (unsigned int)fifo_buffer_b;
		*dma2_control = 0x400000 | DMA_REPEAT | DMA_32 | DMA_SYNC_TO_TIMER | DMA_ENABLE;
	}

	/* set the timer so that it increments once each time a sample is due
	 * we divide the clock (ticks/second) by the sample rate (samples/second)
	 * to get the number of ticks/samples */
	unsigned short ticks_per_sample = CLOCK / sample_rate;

	/* the timers all count up to 65536 and overflow at that point, so we count up to that
	 * now the timer will trigger each time we need a sample, and cause DMA to give it one! */
	*timer0_data = 65536 - ticks_per_sample;

	/* determine length of playback in vblanks
	 * this is the total number of samples, times the number of clock ticks per sample,
	 * divided by the number of machine cycles per vblank (a constant) */
	if (channel == 'A') {
		channel_a_vblanks_remaining = total_samples * ticks_per_sample * (1.0 / CYCLES_PER_BLANK);
		channel_a_total_vblanks = channel_a_vblanks_remaining;
	}
	else if (channel == 'B') {
		channel_b_vblanks_remaining = total_samples * ticks_per_sample * (1.0 / CYCLES_PER_BLANK);
	}


	/* enable the timer */
	*timer0_control = TIMER_ENABLE | TIMER_FREQ_1;
}
/* this function is called each vblank to get the timing of sounds right */
void on_vblank() {


	/* look for vertical refresh */
	//if ((*interrupt_state & INTERRUPT_VBLANK) == INTERRUPT_VBLANK) {

	//VBlankIntrWait();
	/* update channel A */
	if (channel_a_vblanks_remaining == 0) {
		/* restart the sound again when it runs out */
		channel_a_vblanks_remaining = channel_a_total_vblanks;
		*dma1_control = 0;
		*dma1_source = (unsigned int)zelda_treasure_16K_mono;
		*dma1_control = 0x400000 | DMA_REPEAT | DMA_32 |
			IRQ_TIMER0 | DMA_ENABLE;

		/**sound_control &= ~(SOUND_A_RIGHT_CHANNEL | SOUND_A_LEFT_CHANNEL | SOUND_A_FIFO_RESET);
		*dma1_control = 0;*/
	}
	else {
		channel_a_vblanks_remaining--;
	}

	/* update channel B */
	if (channel_b_vblanks_remaining == 0) {
		resetMusic = true;
		/* disable the sound and DMA transfer on channel B */
		*sound_control &= ~(SOUND_B_RIGHT_CHANNEL | SOUND_B_LEFT_CHANNEL | SOUND_B_FIFO_RESET);
		*dma2_control = 0;

	}
	else {
		channel_b_vblanks_remaining--;
	}
	//}

}