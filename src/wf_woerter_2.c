#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "resource_ids.auto.h"

#include "string.h"
#include "stdlib.h"

#include "wf_woerter_2.h"

#define MY_UUID { 0xAD, 0x7F, 0x56, 0xFA, 0x8A, 0xB8, 0x48, 0xF9, 0xA9, 0x9A, 0x6E, 0x83, 0x55, 0x43, 0x0F, 0x40 }

/*
	History
	0.1 initial version
	0.2 added extra space for correctly displaying letter "g"
	0.3 added menu icon
	0.4 fixing bug showing "halb zwölf Mittag" @ 23:30
 */

PBL_APP_INFO(MY_UUID,
	"Wörter 2", "MichaPoe",
	0, 4, /* App version */
	RESOURCE_ID_IMAGE_MENU_ICON,
	APP_INFO_WATCH_FACE);

static struct SimpleWordsData {
	Layer simple_bg_layer;

	Layer date_layer;

	int16_t minute_label_top;
	TextLayer minute_label;
	char minute_buffer[18];

	int16_t qualifier_label_top;
	TextLayer qualifier_label;
	char qualifier_buffer[5];

	int16_t hourqualifier_label_top;
	TextLayer hourqualifier_label;
	char hourqualifier_buffer[12];

	int16_t hour_label_top;
	TextLayer hour_label;
	char hour_buffer[10];

	int16_t daytime_label_top;
	TextLayer daytime_label;
	char daytime_buffer[12];

	Window window;
} s_data;

static void bg_update_proc(Layer* me, GContext* ctx) {
	graphics_context_set_fill_color(ctx, GColorBlack);
	graphics_fill_rect(ctx, me->bounds, 0, GCornerNone);
}

static void date_update_proc(Layer* me, GContext* ctx) {
	(void) me;
	(void) ctx;

	PblTm t;
	get_time(&t);

#ifdef DEBUG
	int h = t.tm_sec / 5;
	int m = t.tm_sec;
	int dt = t.tm_sec % 4;
#else
	int h = t.tm_hour;
	int m = t.tm_min;
	int dt = (t.tm_hour / 6) % 4;
#endif

	int hn = h % 12;
	int mn = m;
	int qualifier = 0;
	int hourqualifier = 0;

	if (m == 0) {
		mn = -1;
	}
	else if (m >= 1 && m <= 14) {
		qualifier = AFTER;
	}
	else if (m == 15) {
		mn = -1;
		hourqualifier = QUARTER;
		hn++;
	}
	else if (m >=16 && m <= 19) {
		qualifier = AFTER;
	}
	else if (m >= 20 && m <=29) {
		mn = 30 - m;
		qualifier = BEFORE;
		hourqualifier = HALF;
		hn++;
	}
	else if (m == 30) {
		mn = -1;
		hourqualifier = HALF;
		hn++;
	}
	else if (m >=31 && m <=39) {
		mn = m - 30;
		qualifier = AFTER;
		hourqualifier = HALF;
		hn++;
	}
	else if (m >= 40 && m <=44) {
		mn = 60 - m;
		qualifier = BEFORE;
		hn++;
	}
	else if (m == 45) {
		mn = -1;
		hourqualifier = THREEFOURTH;
		hn++;
	}
	else if (m >= 46 && m <= 59) {
		mn = 60 - m;
		qualifier = BEFORE;
		hn++;
	}
	else if (m == 60) {
		mn = 0;
	}
	else {
		mn = -1;
	}

	// move hour label upwards if there is no hour qualifier
	//int16_t hqtop = (hourqualifier == 0 ? s_data.hour_label_top : s_data.hourqualifier_label_top);
	//text_layer_init(&s_data.hour_label, GRect(LEFTSPACE, hqtop, TEXTLAYERWIDTH, font_hour_size));

	if (hn > 12) hn -= 12;
	else if (hn == 0) hn += 12;

	if (mn == -1) {
		text_layer_set_text(&s_data.minute_label, EMPTY);
	} else {
		text_layer_set_text(&s_data.minute_label, NUMBERS[mn]);
	}
	text_layer_set_text(&s_data.qualifier_label, QUALIFIER[qualifier]);
	text_layer_set_text(&s_data.hourqualifier_label, HOURQUALIFIER[hourqualifier]);
	if (hourqualifier == 0 && qualifier == 0 && mn == -1) {
		if (h == 12) {
			text_layer_set_text(&s_data.hour_label, DAYTIME_MIDDAY);
			text_layer_set_text(&s_data.daytime_label, EMPTY);
		} else if (h == 0) {
			text_layer_set_text(&s_data.hour_label, DAYTIME_MIDNIGHT1);
			text_layer_set_text(&s_data.daytime_label, DAYTIME_MIDNIGHT2);
		} else {
			text_layer_set_text(&s_data.hour_label, NUMBERS[hn]);
			text_layer_set_text(&s_data.daytime_label, DAYTIME[dt]);
		}
	} else {
		text_layer_set_text(&s_data.hour_label, NUMBERS[hn]);
		text_layer_set_text(&s_data.daytime_label, DAYTIME[dt]);
	}
}

static void handle_init(AppContextRef ctx) {
	(void)ctx;

	window_init(&s_data.window, "MichaPoe words #1");
	window_stack_push(&s_data.window, true);
	resource_init_current_app(&WF_WOERTER_2);

	// resolution: 144 × 168

	// Fonts
	GFont font_gross = fonts_load_custom_font(resource_get_handle(DATEFONTBIG));
	GFont font_mittel = fonts_load_custom_font(resource_get_handle(DATEFONTMIDDLE));
	GFont font_klein = fonts_load_custom_font(resource_get_handle(DATEFONTSMALL));

	GFont font_minute = font_mittel;
	GFont font_qualifier = font_klein;
	GFont font_hour = font_gross;
	GFont font_daytime = font_klein;

	// init layers
	layer_init(&s_data.simple_bg_layer, s_data.window.layer.frame);
	s_data.simple_bg_layer.update_proc = &bg_update_proc;
	layer_add_child(&s_data.window.layer, &s_data.simple_bg_layer);

	// init date layer -> a plain parent layer to create a date update proc
	layer_init(&s_data.date_layer, s_data.window.layer.frame);
	s_data.date_layer.update_proc = &date_update_proc;
	layer_add_child(&s_data.window.layer, &s_data.date_layer);

	// calc top of frames
	int16_t top = TOPSPACE;
	s_data.minute_label_top = top;
	s_data.qualifier_label_top = top += TEXTLAYER_MINUTE_VSPACE + FONT_MINUTE_SIZE;
	s_data.hourqualifier_label_top = top += TEXTLAYER_QUALIFIER_VSPACE + FONT_QUALIFIER_SIZE;
	s_data.hour_label_top = top += TEXTLAYER_QUALIFIER_VSPACE + FONT_QUALIFIER_SIZE;
	s_data.daytime_label_top = top += TEXTLAYER_HOUR_VSPACE + FONT_HOUR_SIZE;
	top += TEXTLAYER_DAYTIME_VSPACE + FONT_DAYTIME_SIZE;
	int16_t move_down = (168 - top) / 2;
	s_data.minute_label_top += move_down;
	s_data.qualifier_label_top += move_down;
	s_data.hourqualifier_label_top += move_down;
	s_data.hour_label_top += move_down;
	s_data.daytime_label_top += move_down;

	// init minute
	text_layer_init(&s_data.minute_label, GRect(LEFTSPACE, s_data.minute_label_top, TEXTLAYERWIDTH, FONT_MINUTE_SIZE + TEXTLAYER_MINUTE_VSPACE));
	text_layer_set_text(&s_data.minute_label, s_data.minute_buffer);
	text_layer_set_background_color(&s_data.minute_label, GColorBlack);
	text_layer_set_text_color(&s_data.minute_label, GColorWhite);
	text_layer_set_font(&s_data.minute_label, font_minute);
	layer_add_child(&s_data.date_layer, &s_data.minute_label.layer);

	// init qualifier
	text_layer_init(&s_data.qualifier_label, GRect(LEFTSPACE, s_data.qualifier_label_top, TEXTLAYERWIDTH, FONT_QUALIFIER_SIZE + TEXTLAYER_QUALIFIER_VSPACE));
	text_layer_set_text(&s_data.qualifier_label, s_data.qualifier_buffer);
	text_layer_set_background_color(&s_data.qualifier_label, GColorBlack);
	text_layer_set_text_color(&s_data.qualifier_label, GColorWhite);
	text_layer_set_font(&s_data.qualifier_label, font_qualifier);
	layer_add_child(&s_data.date_layer, &s_data.qualifier_label.layer);

	// init hourqualifier
	text_layer_init(&s_data.hourqualifier_label, GRect(LEFTSPACE, s_data.hourqualifier_label_top, TEXTLAYERWIDTH, FONT_QUALIFIER_SIZE + TEXTLAYER_QUALIFIER_VSPACE));
	text_layer_set_text(&s_data.hourqualifier_label, s_data.hourqualifier_buffer);
	text_layer_set_background_color(&s_data.hourqualifier_label, GColorBlack);
	text_layer_set_text_color(&s_data.hourqualifier_label, GColorWhite);
	text_layer_set_font(&s_data.hourqualifier_label, font_qualifier);
	layer_add_child(&s_data.date_layer, &s_data.hourqualifier_label.layer);

	// init hour
	text_layer_init(&s_data.hour_label, GRect(LEFTSPACE, s_data.hour_label_top, TEXTLAYERWIDTH, FONT_HOUR_SIZE + TEXTLAYER_HOUR_VSPACE));
	text_layer_set_text(&s_data.hour_label, s_data.hour_buffer);
	text_layer_set_background_color(&s_data.hour_label, GColorBlack);
	text_layer_set_text_color(&s_data.hour_label, GColorWhite);
	text_layer_set_font(&s_data.hour_label, font_hour);
	layer_add_child(&s_data.date_layer, &s_data.hour_label.layer);

	// init daytime
	text_layer_init(&s_data.daytime_label, GRect(LEFTSPACE, s_data.daytime_label_top, TEXTLAYERWIDTH, FONT_DAYTIME_SIZE + TEXTLAYER_DAYTIME_VSPACE));
	text_layer_set_text(&s_data.daytime_label, s_data.daytime_buffer);
	text_layer_set_background_color(&s_data.daytime_label, GColorBlack);
	text_layer_set_text_color(&s_data.daytime_label, GColorWhite);
	text_layer_set_font(&s_data.daytime_label, font_daytime);
	layer_add_child(&s_data.date_layer, &s_data.daytime_label.layer);
}

static void handle_minute_tick(AppContextRef ctx, PebbleTickEvent* t) {
	(void) t;
	layer_mark_dirty(&s_data.window.layer);
}

void pbl_main(void* params) {
	PebbleAppHandlers handlers = {
		.init_handler = &handle_init,
		.tick_info = {
			.tick_handler = &handle_minute_tick,
#ifdef DEBUG
			.tick_units = SECOND_UNIT
#else
			.tick_units = MINUTE_UNIT
#endif
		}
	};
	app_event_loop(params, &handlers);
}

