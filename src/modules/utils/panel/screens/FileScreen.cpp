/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#include "libs/Kernel.h"
#include "Panel.h"
#include "PanelScreen.h"
#include "LcdBase.h"
#include "MainMenuScreen.h"
#include "FileScreen.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include <string>
#include "libs/SerialMessage.h"
#include "StreamOutput.h"
#include "DirHandle.h"
#include "mri.h"

using std::string;
// using namespace std; //delete

FileScreen::FileScreen()
{
//    this->current_folder = ""; //delete
    this->start_play = false;
}

// When entering this screen
void FileScreen::on_enter()
{
    THEPANEL->lcd->clear();

    // Default folder to enter
    // if ( this->current_folder.compare("") == 0 ) { //delete 
    	this->enter_folder("/"); 
    // } else { //delete
    //    this->enter_folder(this->current_folder);
    //}
    // this->enter_folder("/"); //delete
}

void FileScreen::on_exit()
{
    // reset to root directory, I'd prefer not to do this but it crashes on entry next time if it doesn't start at root
    THEKERNEL->current_path= "/";
}

// For every ( potential ) refresh of the screen
void FileScreen::on_refresh()
{
    if ( THEPANEL->menu_change() ) {
        this->refresh_menu();
    }
    if ( THEPANEL->click() ) {
        this->clicked_line(THEPANEL->get_menu_current_line());
    }
}

// Enter a new folder
void FileScreen::enter_folder(const char *folder)
// void FileScreen::enter_folder(std::string folder) //delete
{
    // Remember where we are
    THEKERNEL->current_path= folder;

    // We need the number of lines to setup the menu
    // uint16_t number_of_files_in_folder = this->count_folder_content(); //delete
	current_folder_content.clear();
    this->browse_folder_content(this->current_folder);
	uint16_t number_of_files_in_folder = current_folder_content.size();

    // Setup menu
    THEPANEL->setup_menu(number_of_files_in_folder + 1); // same number of files as menu items
    THEPANEL->enter_menu_mode();

    // Display menu
    this->refresh_menu();
}

// Called by the panel when refreshing the menu, display .. then all files in the current dir
void FileScreen::display_menu_line(uint16_t line)
{
    if ( line == 0 ) {
        THEPANEL->lcd->printf("..");
    } else {
	// THEPANEL->lcd->printf("%s", (this->file_at(line - 1).substr(0, 18)).c_str()); //delete base file
        bool isdir;
        string fn= this->file_at(line - 1, isdir).substr(0, 18);
        if(isdir) {
            if(fn.size() >= 18) fn.back()= '/';
            else fn.append("/");
        }
        THEPANEL->lcd->printf("%s", fn.c_str());
    }
}

// When a line is clicked in the menu, act
void FileScreen::clicked_line(uint16_t line)
{
    if ( line == 0 ) {
        // if ( this->current_folder.compare("/") == 0 ) { //delete
        string path= THEKERNEL->current_path;
        if(path == "/") {
            // Exit file navigation
			current_folder_content.clear();  //local
            THEPANEL->enter_screen(this->parent);
        } else {
            // Go up one folder
            //this->current_folder = this->current_folder.substr(0, this->current_folder.find_last_of('/') + 1); //delete
            //if ( this->current_folder[this->current_folder.length() - 1] == '/' && this->current_folder.length() != 1 ) { //delete
            //    this->current_folder.erase(this->current_folder.length() - 1, 1); //delete
            path = path.substr(0, path.find_last_of('/'));
            if (path.empty()) {
                path= "/";
            }
            //this->enter_folder(this->current_folder); //delete
            this->enter_folder(path.c_str());
        }
    } else {
        // //printf("enter file\r\n"); //delete
        // // Enter file //delete
        // string path = this->current_folder; //delete
        // if ( path.compare("/") == 0 ) { //delete
        //    path = ""; //delete
        // } //delete
        // path = path + "/" + this->file_at( line - 1 ); //delete
        // if ( this->is_a_folder( path ) ) { //delete
        //     this->enter_folder(path); //delete
        // Enter file or dir
        bool isdir;
        string path= absolute_from_relative(this->file_at(line - 1, isdir));
        if(isdir) {
            this->enter_folder(path.c_str());
            return;
        }

        // start printing that file...
		if(this->is_a_gcode(path)) // local
		{ \\local
			this->play_path = path;
			this->start_play = true;
		
    } \\local

} \\localc

//check if a file has .g or .gcode extension \\local
bool FileScreen::is_a_gcode(string path) \\local
{ \\local
	string extension = lc(path.substr(path.find_last_of(".") + 1)); \\local
	if(extension == "g" || extension == "gcode") \\local
		return true; \\local
	else return false; \\localc
} \\local

bool FileScreen::is_no_ext_file(string path) \\local
{ \\local
	if (path.find_last_of(".") == string::npos) return true; \\local
	else return false; \\local
} \\local

// Check wether a line is a folder or a file \\delete
bool FileScreen::is_a_folder( string path ) \\delete
// only filter files that have a .g in them
bool FileScreen::filter_file(const char *f)
{
    /* // In the special case of /local/ ( the mbed flash chip ) we don't have sub-folders, everything is a file
    if ( path.substr(0, 7).compare("/local/") == 0 ) {
        return false;
    }
    // Else, check if it's a folder or not
    DIR *d;
    d = opendir(path.c_str());
    if (d == NULL) {
        return false;
    } else {
        closedir(d);
        return true;
    } */ //delete
    
    string fn= lc(f);
    return (fn.find(".g") != string::npos);
}

// Find the "line"th file in the current folder
// string FileScreen::file_at(uint16_t line) //delete
string FileScreen::file_at(uint16_t line, bool& isdir)
{
	if(current_folder_content.size() > line) return current_folder_content[line];
	else return "";
    /* 
    DIR *d;
    struct dirent *p;
    uint16_t count = 0;
    d = opendir(THEKERNEL->current_path.c_str());
    if (d != NULL) {
        while ((p = readdir(d)) != NULL) {
            // only filter files that have a .g in them and directories
            if((p->d_isdir || filter_file(p->d_name)) && count++ == line ) {
                isdir= p->d_isdir;
                string fn= p->d_name;
                closedir(d);
                return fn;
            }
            //count++; //delete
        } */ //delete
    }

//compares string to banned file names
bool FileScreen::is_acceptable(std::string name)
{
	if(name == "config" || name == "System Volume Information") return false;
	if(is_a_gcode(name) || is_no_ext_file(name))
		return true;
	else return false;
}

/*     if (d != NULL) closedir(d);
    isdir= false;
    return ""; */ //delete

// Browse the current location and push files and folder to vector
void FileScreen::browse_folder_content(std::string folder)

// // Count how many files there are in the current folder that have a .g in them //delete
// uint16_t FileScreen::count_folder_content() //delete
{
    DIR *d;
    struct dirent *p;
    // uint16_t count = 0; //delete
    // d = opendir(folder.c_str()); //delete
    d = opendir(THEKERNEL->current_path.c_str());
    if (d != NULL) {
        while ((p = readdir(d)) != NULL) {
			if(is_acceptable(p->d_name))
            // if(p->d_isdir || filter_file(p->d_name)) count++; //delete some filtr
				current_folder_content.push_back(p->d_name);
        }
        closedir(d);
        // return count; //delete
    }
    // return 0; //delete
}

void FileScreen::on_main_loop()
{
    if (this->start_play) {
        this->start_play = false;
        THEPANEL->set_playing_file(this->play_path);
        // this->play(this->play_path); //delete
        this->play(this->play_path.c_str());
        THEPANEL->enter_screen(this->parent);
        return;
    }
}

// void FileScreen::play(string path) //delete
void FileScreen::play(const char *path)
{
    struct SerialMessage message;
    // message.message = string("play ") + path + " -q"; //delete
    message.message = string("play ") + path;
    message.stream = &(StreamOutput::NullStream);
    THEKERNEL->call_event(ON_CONSOLE_LINE_RECEIVED, &message );
}
