#include<LPC21xx.h>
#include "miniproj_headers.h"

s32 hour,min,sec,date,month,year,day;
extern u8 KEY[4][4];
extern char week[][4];
u32 lastMinutePrinted = 99;

void System_Init(void)
{
		float currentTemp;
		int setpoint=46;
	
		

		//Set the initial time (hours, minutes, seconds)
		SetRTCTimeInfo(12,14,00);
		SetRTCDateInfo(30,10,2025);
		SetRTCDay(4);

	  UARTTxStr("LM35 TEST: \n\r");
		UARTTxStr("Temperature : ");
		UARTTxF32(Read_LM35_NP('C'));
		UARTTxStr(" deg C \n\r");
	
		while(1)
		{
					int pres=0,prev=1;
					
					INPUT:	
					GetRTCTimeInfo(&hour,&min,&sec);
					GetRTCDateInfo(&date,&month,&year);
					GetRTCDay(&day);
					
					DisplayRTCTime(hour,min,sec);
					DisplayRTCDate(date,month,year);
					DisplayRTCDay(day);
					
					currentTemp = Read_LM35_NP('C'); 
					
					DisplayTemp((u32)currentTemp);
					CharLCD(0xDF);
					CharLCD('C');	
					
					if(currentTemp <= setpoint) 
					{ 
							UARTTxStr("[ALERT!] "); 
					}
					UARTTxStr("Temp: "); 
					UARTTxF32(currentTemp); 
					UARTTxStr("\xF8"); 
					UARTTxStr("C @ ");

					UARTTxStr(" ");
					if(hour<10) 
						UARTTxChar('0'); 
					UARTTxU32(hour); 
					UARTTxChar(':');
					if(min<10)  
						UARTTxChar('0'); 
					UARTTxU32(min);  
					UARTTxChar(':');
					if(sec<10)  
						UARTTxChar('0'); 
					UARTTxU32(sec);
					UARTTxStr(" ");
					
					if(date < 10) 
						UARTTxStr("0"); 
					UARTTxU32(date); 
					UARTTxStr("/"); 
					if(month < 10) 
						UARTTxStr("0"); 
					UARTTxU32(month); 
					UARTTxStr("/");
					UARTTxU32(year); 
					
					IODIR1|=(1<<LED);
					if(currentTemp > setpoint) 
					{ 
						UARTTxStr(" - OVER TEMP"); 
						IOCLR1 = 1<<LED;
						IOSET1 = 1<<LED; // LED ON at P0.13 
					} 
					else 
					{ 
						IOCLR1 = 1<<LED; // LED OFF 
					} 
					UARTTxStr("\n\r"); 
					
					PINSEL2 &= ~(1<<SW);
					IODIR1 &= ~(1<<SW);
			
					pres = (((IOPIN1 >> SW)&1)==0);
					
					if(pres && prev)
					{
						delay_ms(50);
						if(pres && prev)
						{
							
							IN1:
							CmdLCD(0x01);
							CmdLCD(0x80);
							StrLCD("1.EDIT RTC INFO");
							CmdLCD(0xC0);
							StrLCD("2.E.SET");
							CmdLCD(0xCA);
							StrLCD("3.EXT");
							
							UARTTxStr("***EDIT MODE ACTIVATED***\n\r");
						
							
							while(1)
							{
								int opt,field,press=0;
								//wait for sw press
								delay_ms(10);//for avoiding keybouncing time.
								
								opt = GetKeyPress();
								
								if(opt ==1)
								{
									IN:
									CmdLCD(0x01);
									CmdLCD(0x80);
									StrLCD("1.H 2.MI 3.S 4.D");
									CmdLCD(0xC0);
									StrLCD("5.M 6.Y 7.DY 8.E");
									
									if(press == 0)
									{
										UARTTxStr("*** Time Editing Mode Activated ***\n\r");
										press++;
									}
									
									field =GetKeyPress();
									
									if(field == 1)
									{
										CmdLCD(0x01);       // Clear LCD
										CmdLCD(0x80);       // 1st line start
										StrLCD("SET HOUR:");
										
										
										while (1)
										{
											int Key=KeyVal();
												if (Key == 15)      // Increase setpoint
												{
														hour++;
														if(hour==24)
														{
															hour=0;
														}
												}
												else if (Key == 10) // Decrease setpoint
												{
																hour--;
													if(hour < 0)
														hour=23;
												}
												else if (Key  == 13) // Exit edit mode
												{
														SetRTCTimeInfo(hour, min, sec);
														CmdLCD(0x01);
														goto IN;
												}

												CmdLCD(0x8A);       // 2nd line
												StrLCD("  ");       // small space
												CmdLCD(0x8A);  
												IntLCD(hour);// Show updated hour

												delay_ms(200);     // small delay to avoid fast repeat
										}	
									}
									else if(field == 2)
									{
										CmdLCD(0x01);       // Clear LCD
										CmdLCD(0x80);       // 1st line start
										StrLCD("SET MIN:");
										
										
										while (1)
										{
											int Key=KeyVal();
												if (Key == 15)      // Increase setpoint
												{
														min++;
														if(min==60)
														{
															min=0;
														}
												}
												else if (Key == 10) // Decrease setpoint
												{
													min--;
													if(min < 0)
														min=59;
												}
												else if (Key  == 13) // Exit edit mode
												{
														SetRTCTimeInfo(hour, min, sec);
														CmdLCD(0x01);
														goto IN;
												}

												CmdLCD(0x8A);       // 2nd line
												StrLCD("  ");       // small space
												CmdLCD(0x8A);  
												IntLCD(min);// Show updated minutes

												delay_ms(200);     // small delay to avoid fast repeat
										}	
									}
									else if(field == 3)
									{
										CmdLCD(0x01);       // Clear LCD
										CmdLCD(0x80);       // 1st line start
										StrLCD("SET SEC:");
										
										
										while (1)
										{
											int Key=KeyVal();
												if (Key == 15)      // Increase setpoint
												{
														sec++;
														if(sec==60)
														{
															sec=0;
														}
												}
												else if (Key == 10) // Decrease setpoint
												{
													sec--;
													if(sec < 0)
														sec=59;
												}
												else if (Key  == 13) // Exit edit mode
												{
														SetRTCTimeInfo(hour, min, sec);
														CmdLCD(0x01);
														goto IN;
												}

												CmdLCD(0x8A);       // 2nd line
												StrLCD("  ");       // small space
												CmdLCD(0x8A);  
												IntLCD(sec);// Show updated seconds

												delay_ms(200);     // small delay to avoid fast repeat
										}	
									}
									else if(field == 4)
									{
										CmdLCD(0x01);       // Clear LCD
										CmdLCD(0x80);       // 1st line start
										StrLCD("SET DATE:");
										
										
										while (1)
										{
											int Key=KeyVal();
												if (Key == 15)      // Increase setpoint
												{
													date++;
												}
												else if (Key == 10) // Decrease setpoint
												{
													date--;
												}
												else if (Key  == 13) // Exit edit mode
												{
														// ----- VALIDATION CHECK -----
														int valid = 1;
														int maxDate;

														if(month == 2)
														{
																if(year % 4 == 0)
																		maxDate = 29;
																else
																		maxDate = 28;
														}
														else if(month == 4 || month == 6 || month == 9 || month == 11)
																maxDate = 30;
														else
																maxDate = 31;

														if(date < 1 || date > maxDate)
																valid = 0;

														// ----- INVALID WARNING -----
														if(!valid)
														{
																CmdLCD(0x01);
																CmdLCD(0x80);
																StrLCD("? INVALID DATE!");
																CmdLCD(0xC0);
																StrLCD("Not Saved");
																UARTTxStr("[WARN] Invalid Date Entered, Not Saved!\n\r");
																delay_ms(1200);
																CmdLCD(0x01);
																goto IN; // return without saving
														}

														// ----- SAVE ONLY IF VALID -----
														SetRTCDateInfo(date, month, year);
														UARTTxStr("[OK] Date Updated Successfully!\n\r");
														CmdLCD(0x01);
														goto IN;
												}

												// ----- CYCLIC LIMITS DURING EDIT -----
												if(month == 2)
												{
														if(year % 4 == 0)
														{
																if(date > 29) date = 1;
																if(date < 1)  date = 29;
														}
														else
														{
																if(date > 28) date = 1;
																if(date < 1)  date = 28;
														}
												}
												else if(month == 4 || month == 6 || month == 9 || month == 11)
												{
														if(date > 30) date = 1;
														if(date < 1)  date = 30;
												}
												else
												{
														if(date > 31) date = 1;
														if(date < 1)  date = 31;
												}

												// ----- DISPLAY UPDATED DATE -----
												CmdLCD(0x8A);
												StrLCD("  ");
												CmdLCD(0x8A);
												IntLCD(date);

												delay_ms(200);
										}
									}
									
									else if(field == 5)
									{
										CmdLCD(0x01);       // Clear LCD
										CmdLCD(0x80);       // 1st line start
										StrLCD("SET MON:");
										
										
										while (1)
										{
											int Key=KeyVal();
												if (Key == 15)      // Increase setpoint
												{
														month++;
														if(month==13)
														{
															month=1;
														}
												}
												else if (Key == 10) // Decrease setpoint
												{
													month--;
													if(month < 0)
														month=12;
												}
												else if (Key  == 13) // Exit edit mode
												{
														SetRTCDateInfo(date,month,year);
														CmdLCD(0x01);
														goto IN;
												}

												CmdLCD(0x8A);       // 2nd line
												StrLCD("  ");       // small space
												CmdLCD(0x8A);  
												IntLCD(month);// Show updated minutes

												delay_ms(200);     // small delay to avoid fast repeat
										}	
									}
									else if(field == 6)
									{
										CmdLCD(0x01);       // Clear LCD
										CmdLCD(0x80);       // 1st line start
										StrLCD("SET YEAR:");
										
										
										while (1)
										{
											
											int Key=KeyVal();
												if (Key == 15)      // Increase setpoint
												{
														year++;
												}
												else if (Key == 10) // Decrease setpoint
												{
													year--;
												}
												else if (Key  == 13) // Exit edit mode
												{
														SetRTCDateInfo(date,month,year);
														CmdLCD(0x01);
														goto IN;
												}

												CmdLCD(0x8A);       // 2nd line
												StrLCD("  ");       // small space
												CmdLCD(0x8A);  
												IntLCD(year);;// Show updated minutes
												
												delay_ms(200);     // small delay to avoid fast repeat
										}	
									}
									else if(field == 7)
									{
										CmdLCD(0x01);       // Clear LCD
										CmdLCD(0x80);       // 1st line start
										StrLCD("SET DAY:");
										
										
										while (1)
										{
											int Key=KeyVal();
												if (Key == 15)      // Increase setpoint
												{
														day++;
														if(day==7)
														{
															day=0;
														}
												}
												else if (Key == 10) // Decrease setpoint
												{
													day--;
													if(day < 0)
														day=6;
												}
												else if (Key  == 13) // Exit edit mode
												{
														SetRTCDay(day);
														CmdLCD(0x01);
														goto IN;
												}

												CmdLCD(0x88);       // 2nd line
												StrLCD("  ");       // small space
												CmdLCD(0x8A);  
												StrLCD((u8*)week[day]);// Show updated minutes

												delay_ms(200);     // small delay to avoid fast repeat
										}	
									}
									else if(field == 8)
									{
										CmdLCD(0x01);
									  goto IN1;
									}
								}	
									
									else if (opt == 2)  // Enter Edit Mode
									{
										int press1=0;
										
										CmdLCD(0x01);       // Clear LCD
										CmdLCD(0x80);       // 1st line start
										StrLCD("SET TEMP:");
										CmdLCD(0xC0);       // 2nd line
										CharLCD(' ');       // small space
										DisplayTemp(setpoint); // Show updated setpoint
										CharLCD(0xDF);     // °
										CharLCD('C');
										
									if(press1 == 0)
									{
										UARTTxStr("*** Set Point Editing Mode Activated ***\n\r");
										press1++;
									}

										while (1)
										{
												int Key = GetKeyPress();
												if (Key == 15)      // Increase setpoint
												{
														setpoint++;
												}
												else if (Key == 10) // Decrease setpoint
												{
														if(setpoint > 0)
																setpoint--;
												}
												else if (Key == 13) // Exit edit mode
												{
														CmdLCD(0x01);
														goto IN1;
												}

												CmdLCD(0xC0);       // 2nd line
												CharLCD(' ');       // small space
												DisplayTemp(setpoint); // Show updated setpoint
												CharLCD(0xDF);     // °
												CharLCD('C');

												delay_ms(200);     // small delay to avoid fast repeat
									  }
								  }
								
								  else if(opt == 3)
								  {
									  CmdLCD(0x01);
									  goto INPUT;
								  }
							  }							
					    }
			      }
	       }
		
}
