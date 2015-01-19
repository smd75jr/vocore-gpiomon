/*
Author: Stephen M. DeLay Jr.
Author email: smd75jr@gmail.com

gpiomon is a simple tool to help assist
with beginning to learn about the GPIO pins
on the Vocore (vocore.io).

The code is based on the blog post by Stefan
at http://gpio.kaltpost.de/?p=2288

The compilation instructions are located
here http://vonger.cn/?p=1585

Feel free to contact me regarding any bugs
or other issues you encounter.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//#define SYSFS_DIR	"/sys/class/gpio/" //the base directory containing the gpio entries

int opt = 0;
bool arg_h = false;
bool arg_p = false;
bool arg_v = false;
int arg_w = 0;
char pin_num;
int pin_val_new;

int zero = 0;

char gpio_value_dir[40] = "/sys/class/gpio/";
int gpio_value_dir_fd;

int main(int argc, char *argv[])
{
	//Get commandline options

	//if no options, print usage information
	//-h print usage information
	//-p "n" = port number, stored in pin_num
	//-v = get current value
	//-w "x" = write value, value given MUST be binary value, stored in pin_val_new

	while ((opt = getopt(argc, argv, "hp:vw:")) != -1) { //process comandline arguments
		switch(opt) {
			case 'h': //no valid arguments
			arg_h = true;
			help();
			break;

			case 'p': //valid arguments are: 0, 1, 2, 7, 13, 14, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25, 26
			arg_p = true;
			// pin_num = (int)optarg;
			pin_num = atoi(optarg);
			// pin_num_check(pin_num);
			printf("\n\npin_num: %d\n", pin_num);
			break;

			case 'v': //no valid arguments
			arg_v = true;
			break;

			case 'w': //valid arguments are: 0, 1
			arg_w = 1;
			// pin_val_new = (int) optarg;
			pin_val_new = atoi(optarg);
			// if ((pin_val_new != zero) && (pin_val_new != 1)) { //sending non-binary values to the gpio pins is probably not a good idea
			// 	printf("Error: Invalid value. Valid values are \"1\" or \"0\"\n");
			// 	exit(0);
			// }
			printf("pin_val_new: %d\n", pin_val_new);
			break;

			case '?':
			if (optopt == 'p') { //prints an error message if user did not give an argument for "p", then terminates
				printf("Error: option \"-p\" requires a GPIO pin number as an argument!\n");
				printf("Try again!\n");
			}
			else if (optopt == 'w') {
				printf("Error: option \"-w\" requires a binary (\"0\" or \"1\") value as an argument!\n");
				printf("Try again!");
			}
			else {
				printf("Error: Invalid option (probably, check your input)");
			}

		}
	}




	char pin_num_char[4];
	sprintf(pin_num_char, "%d", pin_num);
	//itoa(pin_num, pin_num_char, 10);
	//strcpy (pin_num_char, (char)pin_num);


	//gpio_value_dir[30] = "/sys/class/gpio/"; //declare and initialize sysfs directory string
	strcat(gpio_value_dir, "gpio"); //append "gpio" to sysfs directory string
	strcat(gpio_value_dir, pin_num_char); //append the user-specified gpio pin number to sysfs directory string
	strcat(gpio_value_dir, "/value"); //append "/value" to sysfs directory string
	//int gpio_value_dir_fd;
	printf("gpio_value_dir: %s\n", gpio_value_dir);

	gpio_init();

	printf("arg_w: %d\n", arg_w);
	if (arg_w == 1) {
		gpio_set(gpio_value_dir_fd, pin_val_new);
		gpio_term();
	}

}




int gpio_init(void) //initialize the gpio pin (this MUST be done first)
{
	gpio_value_dir_fd = open(gpio_value_dir, O_WRONLY | O_SYNC);
	printf("gpio_value_dir_fd: %d\n", gpio_value_dir_fd);
}

int gpio_term(void) //close the gpio pin (this MUST be done last)
{
	close(gpio_value_dir_fd);
}




int gpio_set(int gpio_vlaue_dir_fd, int pin_val_new) //set $pin_num to value $pin_val_new (MUST be binary value)
{
	write(gpio_value_dir_fd, (void*)pin_val_new, 1); //write the new value to the pin
}




int gpio_get(int gpio_value_dir_fd, int pin_num) //get the current value of gpio pin $pin_num
{
	void* pin_val;
	read(gpio_value_dir_fd, pin_val, 1);
	printf("GPIO%d: %d", pin_num, pin_val); //prints the current value of gpio pin $pin_num in the form "GPIO(pin_num): (pin_val)
}




int pin_num_check(int pin_num)
{
	if (pin_num != 0 && pin_num != 1 && pin_num != 2 && pin_num != 7 && pin_num != 13 && pin_num != 14 && pin_num != 15 && pin_num != 16 && pin_num != 17 && pin_num != 18 && pin_num != 19 && pin_num != 20 && pin_num != 21 && pin_num != 23 && pin_num != 24 && pin_num != 25 && pin_num != 26)
		{
			printf("Error: Invalid pin number!\n");
			printf("The valid pin numbers are:\n");
			printf("0, 1, 2, 7, 13, 14, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25, 26\n");
			exit(0);
		}
}




int help(void)
{

printf("gpiomon v0.1 By: smd75jr (smd75jr@gmail.com)\n\n\n");

printf("gpiomon [options]\n\n");

printf("-h print usage information\n\n");

printf("-p PINNUM\n");
printf("\tSpecify GPIO pin number\n");
printf("\tValid pin numbers are:\n");
printf("\t0, 1, 2, 7, 13, 14, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25, 26\n\n");

printf("-v\n");
printf("\tGet current state of pin specified with -p\n\n");

printf("-w VAL\n");
printf("\tWrite new value VAL to GPIO pin specified with -p\n");
printf("\tValue given MUST be binary (0 or 1) value\n");

}
