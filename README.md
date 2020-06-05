# Serial Console
Sometimes, you need to make not a static Arduino project, but controlled from console (usually, Hyperterminal, included in Windows XP or before, [PuTTY](https://www.putty.org/) or other serial terminal) with user-typed commands.
Because, in dialogs there can be much strings, I recommend to print them from flash directly, without copying it into RAM (like standart compiler do). This saves RAM for something more useful.

# Usage:
	#include "SerialConsole.hpp"

	// create Console object (use Serial1, Serial2 etc, if desired)
	SerialConsole serialConsole(Serial, 80, "#>");

	void setup()
	{
	  // init Serial port (or Serial1, Serial2 etc)
	  Serial.begin(9600);
	}

	void loop()
	{
	  // command received? Process as you wish! For example:
	  if(const char*command = serialConsole.avail())
	  {
	    int parameter;
	    if(strcmp(command,"command1") == 0)
	    {
	      Serial.println("Command one received!");
	    }
	    else if(sscanf(command, "command2 %d", &parameter) == 1)
	    {
	      Serial.print("Command two received with argument ");
	      Serial.println(parameter);
	    }
	    else
	    {
	      Serial.print("Unknown command ");
	      Serial.println(command);
	    }
	    // type promt for user, indicating, that command has been processed
	    serialConsole.done();
	  }
	  // sleep and wait for next interrupt (optional, but the more sleep, the better)
	  MCUCR=(MCUCR & 0B00001111) | 0B10000000; // idle
	  asm("sleep");
	}

	// translate received character to comConsole for processing
	void serialEvent()
	{
	  serialConsole.nextChar();
	}

# Economic usage
	// If you wish to save more RAM, you should use strings, located in flash
	// They are a bit slower, but didn't use RAM, which is available now for something more useful
	// print_P helper function is intended to simplify usage of in-flash strings

	#include "SerialConsole.hpp"
	
	// create Console object (use Serial1, Serial2 etc, if desired)
	SerialConsole serialConsole(Serial, 80, "#>");
	
	void setup()
	{
	  // init Serial port (or Serial1, Serial2 etc)
	  Serial.begin(9600);
	}

	void loop()
	{
	  if(const char*command = serialConsole.avail())
	  {
	    int parameter;
	    if(strcmp_P(command,PSTR("command1")) == 0)
	    {
	      print_P(Serial,PSTR("Command one received!\n"));
	    }
	    else if(sscanf(command, "command2 %d", &parameter) == 1)
	    {
	      print_P(Serial,PSTR("Command two received with argument "));
	      Serial.println(parameter);
	    }
	    else
	    {
	      print_P(Serial,PSTR("Unknown command "));
	      Serial.println(command);
	    }
	    // type promt for user, indicating, that command has been processed
	    serialConsole.done();
	  }
	  // sleep and wait for next interrupt (optional, but the more sleep, the better)
	  MCUCR=(MCUCR & 0B00001111) | 0B10000000; // idle
	  asm("sleep");
	}
	
	void serialEvent()
	{
	  serialConsole.nextChar();
	}
