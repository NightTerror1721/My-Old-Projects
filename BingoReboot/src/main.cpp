#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "core/common.h"
#include "resources/manager.h"

int main()
{
    if (!al_init()) {
        std::cerr << "Failed to initialize Allegro!" << std::endl;
        return -1;
    }

    if (!al_init_primitives_addon()) {
        std::cerr << "Failed to initialize Allegro Primitives Addon!" << std::endl;
        return -1;
    }

    ALLEGRO_DISPLAY* display = al_create_display(800, 600);
    if (!display) {
        std::cerr << "Failed to create display!" << std::endl;
        return -1;
    }

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(100, 100, 300, 300, al_map_rgb(255, 0, 0));
    al_flip_display();

    al_rest(5.0);

    al_destroy_display(display);
    al_shutdown_primitives_addon();
    return 0;
}