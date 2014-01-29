/**
 * A watchface made by olemartinorg
 * (hopefully) ported to Pebble SDK 2.0 by Filip Horvei
 * Lots of help from allan on the Pebble Forums. Thanks! :)
 */
#include <pebble.h>

static Window window;
static GFont font_thin;
static GFont font_thick;

typedef struct {
    TextLayer* layer;
    PropertyAnimation anim;
    const char * text;
    const char * old_text;
} word_t;

static word_t line1;
static word_t line2;
static word_t line3;

static const char * hours[] = {
    "",
    "ett",
    "to",
    "tre",
    "fire",
    "fem",
    "seks",
    "sju",
    "åtte",
    "ni",
    "ti",
    "elleve",
    "tolv"
};

static void norsk_fuzzy(int h, int m, int s){

    if      ((m == 57 && s >= 30) || m > 57) { line1.text = "";         line2.text = "";      h++; }
    else if ((m == 52 && s >= 30) || m > 52) { line1.text = "fem";      line2.text = "på";    h++; }
    else if ((m == 47 && s >= 30) || m > 47) { line1.text = "ti";       line2.text = "på";    h++; }
    else if ((m == 42 && s >= 30) || m > 42) { line1.text = "kvart";    line2.text = "på";    h++; }
    else if ((m == 37 && s >= 30) || m > 37) { line1.text = "ti over";  line2.text = "halv";  h++; }
    else if ((m == 32 && s >= 30) || m > 32) { line1.text = "fem over"; line2.text = "halv";  h++; }
    else if ((m == 27 && s >= 30) || m > 27) { line1.text = "";         line2.text = "halv";  h++; }
    else if ((m == 22 && s >= 30) || m > 22) { line1.text = "fem på";   line2.text = "halv";  h++; }
    else if ((m == 17 && s >= 30) || m > 17) { line1.text = "ti på";    line2.text = "halv";  h++; }
    else if ((m == 12 && s >= 30) || m > 12) { line1.text = "kvart";    line2.text = "over";       }
    else if ((m ==  7 && s >= 30) || m >  7) { line1.text = "ti";       line2.text = "over";       }
    else if ((m ==  2 && s >= 30) || m >  2) { line1.text = "fem";      line2.text = "over";       }
    else {                                     line1.text = "";         line2.text = "";           }

    if (h < 12)  line3.text = hours[h];
    else         line3.text = hours[h - 12];
    if (h == 0 || h == 12)
        line3.text = hours[12];
}

static void display_time(time_t * const ptm){
    line1.old_text = line1.text;
    line2.old_text = line2.text;
    line3.old_text = line3.text;

    norsk_fuzzy(ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

    text_layer_set_text(line1.layer, line1.text);
    text_layer_set_text(line2.layer, line2.text);
    text_layer_set_text(line3.layer, line3.text);
}

static void handle_tick(void){
    if (event->tick_time->tm_sec != 30 &&
        event->tick_time->tm_sec != 0)
        return;

    time_t * ptm = event->tick_time;
    display_time(ptm);
}

void text_layer(word_t * word, GRect frame, GFont font){
    text_layer_set_text(word->layer, "");
    text_layer_set_text_color(word->layer, GColorWhite);
    text_layer_set_background_color(word->layer, GColorClear);
    text_layer_set_font(word->layer, font);
    layer_add_child(window.layer, word->layer.layer);
}

static void handle_init(void){
	
	window = window_create();
    window_stack_push(window, true);
    window_set_background_color(window, GColorBlack);

    font_thin = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_THIN_33));
    font_thick = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_THICK_33));

    text_layer(&line1, GRect(4, 28, 140, 40), font_thin);
    text_layer(&line2, GRect(4, 62, 140, 40), font_thin);
    text_layer(&line3, GRect(4, 97, 140, 40), font_thick);

	tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
}

static void handle_deinit(void){

    fonts_unload_custom_font(font_thin);
    fonts_unload_custom_font(font_thick);
}

int main(void) {
        handle_init();
        app_event_loop();
        handle_deinit();
}
