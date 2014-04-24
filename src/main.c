//Declare and Import references
#include "pebble.h"
#include "pebble_fonts.h"


#define TIME_FRAME        (GRect(0, 25, 144, 130)) 
#define DATE_FRAME        (GRect(0, 3, 144, 30))
#define JOKE_FRAME        (GRect(0, 154, 144, 14)) 

//Declare initial window	
	Window *my_window;    

//Define the layers
	TextLayer *date_layer;   		// Layer for the date
	TextLayer *Time_Layer; 			// Layer for the time
	TextLayer *joke_layer;   		// Layer for the joke


//Define and initialize variables
	//FONTS
	GFont font_date;        // Font for date
	GFont font_time;        // Font for time
	GFont font_joke;        // Font for time


	//Date & Time	
	static char date_text[50];
	static char hour_text[] = "00";
	static char minute_text[] = "00";
	static char time_text[100];
	int ia = 0; 
	int ib = 0;
	int ic = 0;
	int id = 0;
	static char month_text[50];
	static char day_text[]="00";
	int ie = 0;
	int ig = 0;
	int ih = 0;

static const char *HOURS[] = {
	NULL,
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine",
	"ten",
	"eleven", //6
	"twelve",	
};

static char strAlmost[]="almost ";
static char strQuarterPast[]="about quarter past "; //19
static char strThirty[]=" thirtyish "; //19
static char strQuarterTo[]="roughly quarter to ";

static char strEarly[]= "early ";
static char strMid[] = "mid ";
static char strLate[] = "late ";

static char strjoke[]="it's five o'clock somewhere";

//************************//
// Capture the Tick event //
//************************//
void handle_tick(struct tm *tick_time, TimeUnits units_changed)
{

/*
From :54 - :07 "fiveish"
From :08 - :22 "about quarter past five"
From :23 - :37 "five thirtyish"
From :38 - :46 "roughly quarter to six"
From :47 - :53 "almost six"
*/
	
/*
July 1-10 "Early July"
July 11-20 "Mid July"
July 21-31 "Late July"
August 1-10 "Early August"
*/
	
	if (units_changed & MINUTE_UNIT) 
	{

		//Format Date
		//strftime(date_text,sizeof(date_text),"%B %e",tick_time);
		strftime(day_text,sizeof(day_text),"%d",tick_time);
		strftime(month_text,sizeof(month_text),"%B",tick_time);
		ie = day_text[1]-'0';
		ig = day_text[0]-'0';
		ih = ie+(ig*10);
		
		memset(&date_text[0], 0, sizeof(date_text));
		
		if ((ih>0)&&(ih<11)){
			memcpy(&date_text, strEarly, strlen(strEarly)+1);
			strncat(date_text, month_text, strlen(month_text));
		}
		else if ((ih>10)&&(ih<21)){
			memcpy(&date_text, strMid, strlen(strMid)+1);
			strncat(date_text, month_text, strlen(month_text));
		}
		else{
			memcpy(&date_text, strLate, strlen(strLate)+1);
			strncat(date_text, month_text, strlen(month_text));
		}
		
		
		text_layer_set_text(date_layer,date_text); 

		
		//Format time
		
		strftime(hour_text, sizeof(hour_text), "%I", tick_time);
		strftime(minute_text, sizeof(minute_text), "%M", tick_time);
		
		ia = hour_text[1] - '0'; 
		if (hour_text[0]=='1'){ia=ia+10;}	
		
		ib = minute_text[1]-'0';
		ic = minute_text[0]-'0';
		id = ib+(ic*10);
		
		memset(&time_text[0], 0, sizeof(time_text));
		
		if ((id>46)&&(id<54)){
					
			memcpy(&time_text, strAlmost, strlen(strAlmost)+1);
			if(ia==12){strncat(time_text,HOURS[1],strlen(time_text));}
			else{strncat(time_text, HOURS[ia+1], strlen(time_text));}
			}
		else if ((id>53)&&(id<60)){
			
			if(ia==12){memcpy(&time_text,HOURS[1],strlen(HOURS[1])+1);}
			else{memcpy(&time_text, HOURS[ia+1], strlen(HOURS[ia+1])+1);}
			strncat(time_text,"ish",strlen(time_text));
			}
		else if (id<8){

			memcpy(&time_text, HOURS[ia], strlen(HOURS[ia])+1);
			strncat(time_text,"ish",strlen(time_text));
			}
		else if ((id>7)&&(id<23)){
			
			memcpy(&time_text, strQuarterPast, strlen(strQuarterPast)+1);
			strncat(time_text, HOURS[ia], strlen(time_text));
			}
		else if ((id>22)&&(id<38)){
			
			//memcpy(&time_text, strThirty, strlen(strThirty)+1);
			//strncat(time_text, HOURS[ia], strlen(time_text));
			
			memcpy(&time_text, HOURS[ia], strlen(HOURS[ia])+1);
			strncat(time_text,strThirty,strlen(strThirty));
			}
		else if ((id>37)&&(id<47)){
		
		
			memcpy(&time_text, strQuarterTo, strlen(strQuarterTo)+1);
			if (ia==12){strncat(time_text,HOURS[1],strlen(time_text));}
			else{strncat(time_text, HOURS[ia+1], strlen(time_text));}
			}
		else{memcpy(&time_text, HOURS[ia], strlen(HOURS[ia])+1);}
	
	
  		//text_layer_set_text(Time_Layer, HOURS[ia]);
  		text_layer_set_text(Time_Layer, time_text);



	} //MINUTE CHANGES

} //HANDLE_TICK 



//****************************//
// Initialize the application //
//****************************//

void handle_init(void)
{

	ResHandle res_t;
	
	//Create the main window
	my_window = window_create(); 
	window_stack_push(my_window, true /* Animated */);
	window_set_background_color(my_window, GColorBlack);

	
	//res_t = resource_get_handle(RESOURCE_ID_HELVETICA_30); // Time font
	
    font_time = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
	//font_time = fonts_load_custom_font(res_t);
	 
	font_date = fonts_get_system_font(FONT_KEY_GOTHIC_18);
	font_joke = fonts_get_system_font(FONT_KEY_GOTHIC_14);


	//LOAD THE LAYERS

		//Display the Time layer
		Time_Layer = text_layer_create(TIME_FRAME);
		text_layer_set_text_color(Time_Layer, GColorWhite);
		text_layer_set_background_color(Time_Layer, GColorClear);
		text_layer_set_font(Time_Layer, font_time);
		text_layer_set_text_alignment(Time_Layer, GTextAlignmentCenter);
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(Time_Layer)); 

		//Display the Date layer
		date_layer = text_layer_create(DATE_FRAME);
		text_layer_set_text_color(date_layer, GColorWhite);
		text_layer_set_background_color(date_layer, GColorClear);
		text_layer_set_font(date_layer, font_date);
		text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(date_layer)); 
	
		//Display the Joke layer
		joke_layer = text_layer_create(JOKE_FRAME);
		text_layer_set_text_color(joke_layer, GColorWhite);
		text_layer_set_background_color(joke_layer, GColorClear);
		text_layer_set_font(joke_layer,font_joke);
		text_layer_set_text_alignment(joke_layer, GTextAlignmentCenter);
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(joke_layer)); 

  		text_layer_set_text(joke_layer,strjoke);
	
	// Ensures time is displayed immediately (will break if NULL tick event accessed).
	  // (This is why it's a good idea to have a separate routine to do the update itself.)

		time_t now = time(NULL);
	  	struct tm *current_time = localtime(&now);
		handle_tick(current_time, MINUTE_UNIT);
		tick_timer_service_subscribe(MINUTE_UNIT, &handle_tick);


} //HANDLE_INIT



//**********************//
// Kill the application //
//**********************//
void handle_deinit(void)
{
  //text_layer_destroy(text_layer);

	//Unsuscribe services
	tick_timer_service_unsubscribe();

	//Deallocate layers
	text_layer_destroy(Time_Layer);
	text_layer_destroy(date_layer);

	//Deallocate the main window
  	window_destroy(my_window);

} //HANDLE_DEINIT


//*************//
// ENTRY POINT //
//*************//
int main(void) 
{	
	handle_init();
	app_event_loop();
	handle_deinit();
}