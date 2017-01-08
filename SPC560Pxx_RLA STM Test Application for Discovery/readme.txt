In this example, using STM driver, 4 timers are set to expire 
respectively after 1,2,3 and 4 seconds.

Each timers has its own callback defined in main.c which will
send a string to serial port when expired.

Please connect serial cable and set baud rate to 38400 before 
run this test application.