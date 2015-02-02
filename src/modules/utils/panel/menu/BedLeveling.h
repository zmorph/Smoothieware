

std::string G0_XY(float x, float y)
{
	char buffer[15];
	snprintf(buffer, sizeof(buffer), "G0 X%4.0f Y%4.0f", x, y);
	return std::string(buffer);
} 

void go_to_front_left_corner()
{
	send_gcode("G28 XYZ");
	send_gcode("G0 Z5");
	send_gcode("G0 X10 Y10");
	send_gcode("G0 Z0");
}

void go_to_front_right_corner()
{
	send_gcode("G0 Z5");
	send_gcode(G0_XY(info::settings.x_max-10, 10));
	send_gcode("G0 Z0");
}

void go_to_back_right_corner()
{
	send_gcode("G0 Z5");
	send_gcode(G0_XY(info::settings.x_max-10, info::settings.y_max-10));
	send_gcode("G0 Z0");
}

void go_to_back_left_corner()
{
	send_gcode("G0 Z5");
	send_gcode(G0_XY(10, info::settings.y_max-10));
	send_gcode("G0 Z0");
}

void extrude()
{
	send_gcode("G91");
	send_gcode("G1 E5 F100");
	send_gcode("G90");
}

void retract()
{
	send_gcode("G91");
	send_gcode("G1 E-5 F100");
	send_gcode("G90");
}

CompositeItem LOCATION bed_leveling_items[] =
{
	ui::Command(i18n::bed_leveling_caption, enque<go_to_front_left_corner>)
};

CompositeItem LOCATION front_left_corner_calibration_items[] =
{
	ui::Command(i18n::front_left_corner_caption, enque<go_to_front_right_corner>)
};

CompositeItem LOCATION front_right_corner_calibration_items[] =
{
	ui::Command(i18n::front_right_corner_caption, enque<go_to_back_right_corner>)
};

CompositeItem LOCATION back_right_corner_calibration_items[] =
{
	ui::Command(i18n::back_right_corner_caption, enque<go_to_back_left_corner>)
};

CompositeItem LOCATION back_left_corner_calibration_items[] =
{
	ui::Command(i18n::back_left_corner_caption, enque<go_to_front_left_corner>)
};

CompositeItem LOCATION front_left_corner_again_calibration_items[] =
{
	ui::Command(i18n::front_left_corner_again_caption, enque<go_to_front_right_corner>)
};

CompositeItem LOCATION front_right_corner_again_calibration_items[] =
{
	ui::Command(i18n::front_right_corner_again_caption, enque<go_to_back_right_corner>)
};

CompositeItem LOCATION back_right_corner_again_calibration_items[] =
{
	ui::Command(i18n::back_right_corner_again_caption, enque<go_to_back_left_corner>)
};

CompositeItem LOCATION back_left_corner_again_calibration_items[] =
{
	ui::Command(i18n::back_left_corner_again_caption, enque<go_to_front_left_corner>)
};

ui::Widget LOCATION bed_leveling_widget(&splash_text_layout);
ui::Widget LOCATION front_left_corner_calibration_widget(&splash_text_layout);
ui::Widget LOCATION front_right_corner_calibration_widget(&splash_text_layout);
ui::Widget LOCATION back_right_corner_calibration_widget(&splash_text_layout);
ui::Widget LOCATION back_left_corner_calibration_widget(&splash_text_layout);
ui::Widget LOCATION front_left_corner_again_calibration_widget(&splash_text_layout);
ui::Widget LOCATION front_right_corner_again_calibration_widget(&splash_text_layout);
ui::Widget LOCATION back_right_corner_again_calibration_widget(&splash_text_layout);
ui::Widget LOCATION back_left_corner_again_calibration_widget(&splash_text_layout);

ui::Link LOCATION bed_leveling_links[1];
ui::Link LOCATION front_left_corner_calibration_links[1];
ui::Link LOCATION front_right_corner_calibration_links[1];
ui::Link LOCATION back_right_corner_calibration_links[1];
ui::Link LOCATION back_left_corner_calibration_links[1];
ui::Link LOCATION front_left_corner_again_calibration_links[1];
ui::Link LOCATION front_right_corner_again_calibration_links[1];
ui::Link LOCATION back_right_corner_again_calibration_links[1];
ui::Link LOCATION back_left_corner_again_calibration_links[1];

ui::Group LOCATION bed_leveling_menu(bed_leveling_items, bed_leveling_links, bed_leveling_widget);
ui::Group LOCATION front_left_corner_calibration_menu(front_left_corner_calibration_items, front_left_corner_calibration_links, front_left_corner_calibration_widget);
ui::Group LOCATION front_right_corner_calibration_menu(front_right_corner_calibration_items, front_right_corner_calibration_links, front_right_corner_calibration_widget);
ui::Group LOCATION back_right_corner_calibration_menu(back_right_corner_calibration_items, back_right_corner_calibration_links, back_right_corner_calibration_widget);
ui::Group LOCATION back_left_corner_calibration_menu(back_left_corner_calibration_items, back_left_corner_calibration_links, back_left_corner_calibration_widget);
ui::Group LOCATION front_left_corner_again_calibration_menu(front_left_corner_again_calibration_items, front_left_corner_again_calibration_links, front_left_corner_again_calibration_widget);
ui::Group LOCATION front_right_corner_again_calibration_menu(front_right_corner_again_calibration_items, front_right_corner_again_calibration_links, front_right_corner_again_calibration_widget);
ui::Group LOCATION back_right_corner_again_calibration_menu(back_right_corner_again_calibration_items, back_right_corner_again_calibration_links, back_right_corner_again_calibration_widget);
ui::Group LOCATION back_left_corner_again_calibration_menu(back_left_corner_again_calibration_items, back_left_corner_again_calibration_links, back_left_corner_again_calibration_widget);