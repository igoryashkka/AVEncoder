// -------------------------------------------------------- TIM1 IC callback --------------------------------------------------------------- //
// --------
// 1-us is 24-ticks 
// period of line is 64us (1536 in ticks), 1 line -> 59-60us
//
#define MAX_TICKS_PERIO_LINE 1488
#define MIN_TICKS_PERIO_LINE 1478
//even - 0
//odd - 1
int flag_evenodd = 0;

volatile uint32_t currentCaptureValue = 0;
volatile uint32_t previousCaptureValue = 0;
//volatile uint32_t counter_h = 0;
//volatile uint32_t counter_line_period = 0;
//volatile uint32_t counter_v = 0;
volatile uint8_t even_frame_flag = 0;
#define COUNETR_LINE 305
#define SIX_TIMES_VERTICAL_SYNC 6

//Flags starting detection of impulses
// NOT detect - 0
// Can detect - 1
int flag_start_detect_line = 0;
int flag_start_detect_h_sync = 0;
int flag_start_detect_half = 1;



int times_count_h_sync = 0;
int possible_times_count_h_sync = 1;

int times_changed_even = 0;
int times_changed_odd = 0;


void APP_InputcaptureCallback(void) {
    currentCaptureValue = LL_TIM_IC_GetCaptureCH1(TIM1);
    uint32_t pulseWidth;
    if (currentCaptureValue >= previousCaptureValue) {
        // no overflow
        pulseWidth = currentCaptureValue - previousCaptureValue;
    } else {
        // overflow
        pulseWidth = (currentCaptureValue + (TIM1->ARR + 1)) - previousCaptureValue;
    }
		
		/* - Hnadling mesuremnt of impulses			
				- Counting lines and v-sync
				1. Search first v_sync(2.4-3.5us),count up to 10.
				2. Search first line (59.5-61.0us), count up 305 (always impulses with timings as (59.5-61.0us) must be 305).
				3. After completing of 305 lines again satrt serch of first vertical-sync.
				
				- Handling mesuremnt of EVEN/ODD filed 
				1. Reapaet **Counting Lines...** Steps.
				2. Start count h-sync(4.7us) when get first v-sync up to first line.
				3. Start count half-line(27.0-29.0us) when get 6th v-sync.
				4. If next half-line counter == 4 its 1st or EVEN frame | if counter == 5 its 2nd or ODD frame		
		*/
		
		  // v_sync (2.5 us)
    if (pulseWidth >= 55 && pulseWidth <= 80){
        counter_v++;
				
				flag_start_detect_line = 1;
			
				if (counter_v >= SIX_TIMES_VERTICAL_SYNC){
					flag_start_detect_half = 1;
				}
				
				//Uncoment to test counting of counetr_h, must be 6 if possible_times_count_h_sync is 1.
				// 																				must be 12 if possible_times_count_h_sync is 2 etc... 		
				/*
				if(times_count_h_sync<possible_times_count_h_sync){
						flag_start_detect_h_sync = 1;			
						times_count_h_sync++;
				}	
				*/
    }
    
    
    // line period (59 us)
    if (pulseWidth >= 1478 && pulseWidth <= 1488 && flag_start_detect_line) { 
        counter_line++;   
				counter_half = 0;
				flag_start_detect_h_sync = 0;
				flag_start_detect_half = 0;
				if(counter_line==COUNETR_LINE){
					flag_start_detect_line = 0;
					counter_line = 0;
					counter_v = 0;
				}
				
    }
		// half-line (27-29us)
		if (pulseWidth >= 670 && pulseWidth <= 710 && flag_start_detect_half) { 
				counter_half++;
			
				if(counter_half == 4){
					flag_evenodd = 1;
					times_changed_odd++;
				}else if(counter_half == 5){
					flag_evenodd = 0;
					times_changed_even++;
				}
			
     } 
		
		// h_sync (4.7 us)
    if (pulseWidth >= 110 && pulseWidth <= 120 && flag_start_detect_h_sync) { 
        counter_h++;						
    }
		
    previousCaptureValue = currentCaptureValue; 
}


void TIM1_CC_IRQHandler(void)
{
  if(LL_TIM_IsEnabledIT_CC1(TIM1) == 1)
  {
    LL_TIM_ClearFlag_CC1(TIM1);
    
    APP_InputcaptureCallback();
  }
}