This is the project for BOTH MCU curriculum design and Electronic Design Contest.

===ABOUT Version===

Version 1.0
Digital Tube Demenstration: abstract codes.
Beep Sound: abstract codes.
Key Scan: abstract codes.

Version 1.1
Digital Tube Demenstration: function well.
Beep Sound: funtion well.
Key Scan: almost function well.

Version 1.2
Implement Function: frequency measure.

Version 1.3
Fix bug of tube demonstration.

Version 2.0
Fix a bug about TIMER counting and implement function: phase measure.
Actually, after 2 days debuging I just rewrite a single following line of code, 
TIM2TIM5_DIFF_VAL=TIM5->CNT; //TIM5->CCR1
and the whole project which contains thousands of lines of codes suddlenly becomes full of color.
