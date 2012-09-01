#include "window.h"
#include <gtkmm/main.h>

int main(int argc, char *argv[])
{
    Glib::thread_init();
    Gtk::Main kit(argc, argv);
    KnapsackBackEnd window;
    Gtk::Main::run(window);
    return 0;
}
