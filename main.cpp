#include <sstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include "npc.h"
#include "Player.h"
#include "Enemigo1.h"
#include "Enemigo2.h"
#include "Enemigo3.h"
#include "Enemigo4.h"
#include <cmath>

using namespace std;

#define SW 800
#define SH 600
#define UPS 60.0
#define FPS 8.0

ALLEGRO_DISPLAY * display;
ALLEGRO_FONT *font;
ALLEGRO_BITMAP *player[17], *bg, *infection[10], *planets[10], *destroyed[10], *fire, *fire2, *fire3, *fire4, *ene1,*ene2,*ene3,*ene4, *galaxy[17], *sparks[12];
ALLEGRO_SAMPLE *music, *sfx;
ALLEGRO_TIMER *timer;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_KEYBOARD_STATE keyState;

bool done = false, draw = true, infected = false, nivel1 = true, nivel2 = false, nivel3 = false, fire11 = false, fire22=true, secreto=false, iniciar = false, menu1=false;
int lives = 6, x = 0, y = 0, infected_counter = 0, player_counter = 0, planet_counter_x = 200, planet_counter_y = -300, planet = 0,
destruido1_x = 200, destruido1_y = -200,destruido2_x = 200, destruido2_y = -200, destruido1 = 0, destruido2 = 0, player_width = 100, enemigos = 0,
fire111 = 0, fire222 = 0, pos_x=0, player_health = 0, explotion = 0, sparky = 0, pos_enemigo_x=0, pos_enemigo_y=0;
Player plr; Enemigo1 enemy1; Enemigo2 enemy2; Enemigo3 enemy3; Enemigo4 enemy4;
int randomizer()
{
    return 0+rand()%(9-0);
}

int randomizer2()
{
    return 0+rand()%(650-0);
}

bool collision(float x, float y, float ex, float ey, int width, int height)
{
    if(x + width < ex || x > ex + width || y + height < ey || y > ey + height)
    {
       return false;
    }
    else
    {
        return true;
    }
}

void INFECTED()
{
    for(int i = 0; i<10; i++)
    {
        stringstream str;
        str<<"assets/effects/infected/fussy"<<i+1<<".png";
        infection[i] = al_load_bitmap(str.str().c_str());
    }
}

void initPlayer()
{
    for(int i = 0; i<17; i++)
    {
        stringstream str;
        str<<"assets/ships/special/blue/"<<i+1<<".png";
        player[i] = al_load_bitmap(str.str().c_str());
    }
    fire = al_load_bitmap("assets/lasers/laserBlue01.png");
    player_health = plr.vida;
}

void initPlanets()
{
    for(int i = 0; i<10; i++)
    {
        stringstream str;
        str<<"assets/planets/planeta ("<<i+1<<").png";
        planets[i] = al_load_bitmap(str.str().c_str());
    }
    for(int i = 0; i<10; i++)
    {
        stringstream str;
        str<<"assets/planets/damage ("<<i+1<<").png";
        destroyed[i] = al_load_bitmap(str.str().c_str());
    }
}
void initEnemies()
{
    fire2=al_load_bitmap("assets/lasers/laserRed01.png");
    fire3=al_load_bitmap("assets/lasers/laserRed01.png");
    fire4=al_load_bitmap("assets/lasers/laserRed01.png");
    if(nivel1==true)
    {
        nivel2 = false;
        nivel3 = false;
        secreto = false;
        enemigos = enemy1.vida;
        ene1 = al_load_bitmap("assets/ships/level1_enemy1.png");
    }
    if(nivel2==true)
    {
        nivel1 = false;
        nivel3 = false;
        secreto = false;
        enemigos = enemy2.vida;
        ene2 = al_load_bitmap("assets/ships/level2_enemy1.png");
    }
    if(nivel3==true)
    {
        nivel2 = false;
        nivel1 = false;
        secreto = false;
        enemigos = enemy3.vida;
        ene3 = al_load_bitmap("assets/ships/level3_enemy1.png");
    }
    if(secreto==true)
    {
        nivel2 = false;
        nivel3 = false;
        nivel1 = false;
        enemigos = enemy4.vida;
        ene4 = al_load_bitmap("assets/ships/level4_enemy1.png");
    }
}

void initEffects()
{
    for(int i = 0; i<17; i++)
    {
        stringstream str;
        str<<"assets/effects/galaxy_effect/galaxy ("<<i+1<<").png";
        galaxy[i] = al_load_bitmap(str.str().c_str());
    }
    for(int i = 0; i<12; i++)
    {
        stringstream str;
        str<<"assets/effects/sparks/sparks ("<<i+1<<").png";
        sparks[i] = al_load_bitmap(str.str().c_str());
    }
}
void initLevel()
{
    if(menu1==true)
    {
    bg = al_load_bitmap("assets/backgrounds/menu.png");
    music = al_load_sample("assets/sounds/unlock.ogg");
    }
    if(nivel1==true)
    {
        bg = al_load_bitmap("assets/backgrounds/Background.png");
        music = al_load_sample("assets/sounds/unlock.ogg");
    }
    if(nivel2==true)
    {
        bg = al_load_bitmap("assets/backgrounds/Gods-and-Idols-2012-04-11-21-40-17-86.jpg");
        music = al_load_sample("assets/sounds/curse_the_night.ogg");
    }
    if(nivel3==true)
    {
        bg = al_load_bitmap("assets/backgrounds/Gods-and-Idols-2012-04-11-21-41-43-84.jpg");
        music = al_load_sample("assets/sounds/infinite_railroad.ogg");
    }
    if(secreto==true)
    {
        bg = al_load_bitmap("assets/backgrounds/SpaaaaaaaceFinFlatWithRing.jpg");
        music = al_load_sample("assets/sounds/midnight_shore.ogg");
    }

}

void initialize()
{
    al_init();

    al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(SW,SH);
    al_set_window_title(display, "C-Type");

    al_install_keyboard();
    al_install_mouse();
    al_init_image_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_init_font_addon();
    al_init_ttf_addon();

//    font = al_load_font("assets/OrbitronBlack.ttf", 36, NULL);
//    al_draw_text(font, al_map_rgb(44, 117, 255), SW/2,SH/2, ALLEGRO_ALIGN_CENTRE, "This is a test");

    initPlayer();
    initPlanets();
    initEffects();

    initEnemies();
    initLevel();

    INFECTED();


    sfx = al_load_sample("assets/sounds/sfx_laser1.ogg");
    al_reserve_samples(2);
    al_play_sample(music,1.0,0.0,1.0,ALLEGRO_PLAYMODE_LOOP, 0);

    timer = al_create_timer(1.0/UPS);

    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    //al_hide_mouse_cursor(display);

    al_start_timer(timer);
}

void handleEvents(ALLEGRO_EVENT *event)
{
    if(event->keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                done = true;
    if(event->keyboard.keycode == ALLEGRO_KEY_SPACE && fire11 == false)
    {
        al_play_sample(sfx,3.0,0.0,2.0,ALLEGRO_PLAYMODE_ONCE,0);
        fire11 = true;
        pos_x=plr.pos[0]+50;
    }
    if(event-> type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        x = event->mouse.x;
        y = event->mouse.y;
    }
//    else if(event-> type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
//    {
//        if(event->mouse.button & 1)
//
//        else if (event->mouse.button & 2)
//
//    }
}

void update()
{
//
//    if(al_key_down(&keyState, ALLEGRO_KEY_W))
//    {
//       plr.pos[1] -= plr.speed;
//    }
//    else if(al_key_down(&keyState, ALLEGRO_KEY_S))
//    {
//        plr.pos[1] += plr.speed;
//    }
    if(al_key_down(&keyState, ALLEGRO_KEY_D) && plr.pos[0]<(800-player_width))
    {
        plr.pos[0] += plr.speed;
    }
    else if(al_key_down(&keyState, ALLEGRO_KEY_A) && plr.pos[0]>0)
    {
        plr.pos[0] -= plr.speed;
    }


    draw = true;
}

void renderSparks(int x, int y)
{
    if(sparky<12)
    {
        al_draw_bitmap(sparks[sparky],x,y,NULL);
        sparky++;
    }else
        sparky = 0;
}

void renderExplosion(int x, int y)
{
        if(explotion<17)
        {
            al_draw_bitmap(galaxy[explotion],x,y,NULL);
            explotion++;
        }else
        {
            //al_rest(2.0);
            if(nivel1)
            {
                nivel2=true;
                nivel1=false;
            }else if(nivel2)
            {
                nivel3=true;
                nivel2=false;
            }else if(nivel3)
            {
                secreto=true;
                infected=true;
                nivel3=false;
            }
            explotion = 0;
            done=true;
        }
}

void playerRender()
{
   al_draw_bitmap(player[player_counter], plr.pos[0], plr.pos[1], NULL);
    player_counter++;
    if(nivel1==true && player_counter==7)
    {
        player_counter=0;
        player_width = 100;
    }
    if(nivel2==true && player_counter==12)
    {
        player_counter=8;
         player_width = 125;
    }
    if(nivel3==true && player_counter==16)
    {
        player_counter=13;
         player_width = 125;
    }
    if(secreto==true && player_counter==16)
    {
        player_counter=0;
         player_width = 125;
    }
    if(fire11)
    {
        al_draw_bitmap(fire,pos_x ,plr.pos[1]-fire111,NULL);
        fire111+=20 ;
        if(fire111>750)
        {
            fire11 = false;
            fire111=0;
        }
        if(collision(pos_x,plr.pos[1]-fire111,enemy1.pos[0],enemy1.pos[1],al_get_bitmap_width(ene1),al_get_bitmap_height(ene1)))
        {
            enemigos--;
            cout<<enemigos<<endl;
        }
    }
}

void displacement()
{
    if(planet_counter_y == -300)
    {
        planet = randomizer();
        planet_counter_x = randomizer2();
    }
    if(planet_counter_y>=-300)
    {
        al_draw_bitmap(planets[planet],planet_counter_x,planet_counter_y,NULL);
        planet_counter_y+=planet/3;
    }
    if(planet_counter_y>=800)
        planet_counter_y = -300;

    if(destruido1_y == -200)
    {
        destruido1 = randomizer();
        destruido1_x = randomizer2();
    }
    if(destruido1_y>=-200)
    {
       al_draw_bitmap(destroyed[destruido1],destruido1_x, destruido1_y,NULL);
       destruido1_y+=destruido1/2;
    }
    if(destruido1_y>=800)
        destruido1_y = -200;

    if(destruido2_y == -200)
    {
        destruido2 = randomizer();
        destruido2_x = randomizer2();
    }
    if(destruido2_y>=-200)
    {
       al_draw_bitmap(destroyed[destruido2],destruido2_x, destruido2_y,NULL);
       destruido2_y+=destruido2/2;
    }
    if(destruido2_y>=800)
        destruido2_y = -200;
}

void renderInfection()
{
    if(infected==true)
    {
        infected_counter = randomizer();
        al_draw_bitmap(infection[infected_counter], 0,0,NULL);
    }
}

void disparar(int x, int y)
{
    if(fire22)
    {
        al_draw_bitmap(fire2,x+50,y+fire222,NULL);
        al_draw_bitmap(fire3,x,y+fire222,NULL);
        al_draw_bitmap(fire4,x-50,y+fire222,NULL);
        fire222+=20 ;
        if(fire222>750)
        {
            //fire22 = false;
            fire222=0;
        }
        if(collision(x,y+fire222,plr.pos[0],plr.pos[1],al_get_bitmap_width(player[1]),al_get_bitmap_height(player[1])))
        {
            player_health--;
            cout<<player_health<<endl;
        }
    }
}

void renderEnemies()
{
    if(nivel1==true)
    {
        if(enemigos<enemy1.vida)
            renderSparks(enemy1.pos[0],enemy1.pos[1]);
        if(enemigos>0)
        {
            al_draw_bitmap(ene1,enemy1.pos[0],enemy1.pos[1],NULL);
            disparar(enemy1.pos[0],enemy1.pos[1]);
        }
        else
            renderExplosion(enemy1.pos[0],enemy1.pos[1]);
    }
    if(nivel2==true)
    {
        if(enemigos<enemy2.vida)
            renderSparks(enemy2.pos[0],enemy2.pos[1]);
        if(enemigos>0)
        al_draw_bitmap(ene2,enemy2.pos[0],enemy2.pos[1],NULL);
        else
            renderExplosion(enemy2.pos[0],enemy2.pos[1]);
    }
    if(nivel3==true)
    {
        if(enemigos<enemy3.vida)
            renderSparks(enemy3.pos[0],enemy3.pos[1]);
        if(enemigos>0)
        al_draw_bitmap(ene3,enemy3.pos[0],enemy3.pos[1],NULL);
        else
            renderExplosion(enemy3.pos[0],enemy3.pos[1]);
    }
    if(secreto==true)
    {
        if(enemigos<enemy4.vida)
            renderSparks(enemy4.pos[0],enemy4.pos[1]);
        if(enemigos>0)
        al_draw_bitmap(ene4,enemy4.pos[0],enemy4.pos[1],NULL);
        else
            renderExplosion(enemy4.pos[0],enemy4.pos[1]);
    }
}

void render()
{
     draw = false;

    al_draw_bitmap(bg, 0,0,NULL);
    displacement();
    playerRender();
    renderEnemies();
    renderInfection();
    al_flip_display();
    al_clear_to_color(al_map_rgb(0,0,0));
}

void terminate_game()
{
    al_destroy_display(display);
    al_destroy_font(font);
    for(int i = 0; i<17; i++)
    {
        al_destroy_bitmap(player[i]);
        al_destroy_bitmap(galaxy[i]);
    }
    al_destroy_bitmap(bg);
    for(int i = 0; i<10; i++)
    {
       al_destroy_bitmap(infection[i]);
       al_destroy_bitmap(planets[i]);
       al_destroy_bitmap(destroyed[i]);
    }
    al_destroy_bitmap(ene1);
    al_destroy_bitmap(ene2);
    al_destroy_bitmap(ene3);
    al_destroy_bitmap(ene4);
    al_destroy_bitmap(fire);
    al_destroy_bitmap(fire2);

    for(int i = 0; i<12; i++)
    {
        al_destroy_bitmap(sparks[i]);
    }
    al_destroy_timer(timer);
    al_destroy_sample(music);
    al_destroy_sample(sfx);
    al_destroy_event_queue(event_queue);
}

void highScore()
{

}

void instructions()
{
    bg = al_load_bitmap("assets/backgrounds/instrucciones.png");
    al_draw_bitmap(bg, 0,0,NULL);
    al_flip_display();
    al_rest(5.0);
    bg = al_load_bitmap("assets/backgrounds/menu.png");
}
void menu()
{
    bg = al_load_bitmap("assets/backgrounds/menu.png");
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_draw_bitmap(bg, 0,0,NULL);
    al_flip_display();
    al_clear_to_color(al_map_rgb(0,0,0));
    ALLEGRO_EVENT event;
      al_wait_for_event(event_queue, &event);
      al_get_keyboard_state(&keyState);
      if(event.type == ALLEGRO_EVENT_KEY_DOWN)
          if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
          {
              done = true;
              iniciar = true;
              terminate_game();
          }
          if(event.keyboard.keycode == ALLEGRO_KEY_I)
                    instructions();
          if(event.keyboard.keycode == ALLEGRO_KEY_H)
            highScore();
          if(event.keyboard.keycode == ALLEGRO_KEY_ENTER)
          {
              nivel1=true;
              menu1=false;
              iniciar=true;
          }
}

int main()
{

    initialize();

    while(!iniciar)
    {
        menu();

    }
    initEnemies();
    initLevel();
    while(!done)
    {
      ALLEGRO_EVENT event;
      al_wait_for_event(event_queue, &event);
      al_get_keyboard_state(&keyState);

      if(event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_MOUSE_AXES || event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
          handleEvents(&event);

      if(event.type == ALLEGRO_EVENT_TIMER)
        update();

     if(draw)
        render();
    }
    done = false;
    initEnemies();
    initLevel();
    while(!done)
    {
      ALLEGRO_EVENT event;
      al_wait_for_event(event_queue, &event);
      al_get_keyboard_state(&keyState);

      if(event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_MOUSE_AXES || event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
          handleEvents(&event);

      if(event.type == ALLEGRO_EVENT_TIMER)
        update();

     if(draw)
        render();
    }
    done = false;
    initEnemies();
    initLevel();
    while(!done)
    {
      ALLEGRO_EVENT event;
      al_wait_for_event(event_queue, &event);
      al_get_keyboard_state(&keyState);

      if(event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_MOUSE_AXES || event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
          handleEvents(&event);

      if(event.type == ALLEGRO_EVENT_TIMER)
        update();

     if(draw)
        render();
    }
    done = false;
    initEnemies();
    initLevel();
    while(!done)
    {
      ALLEGRO_EVENT event;
      al_wait_for_event(event_queue, &event);
      al_get_keyboard_state(&keyState);

      if(event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_MOUSE_AXES || event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
          handleEvents(&event);

      if(event.type == ALLEGRO_EVENT_TIMER)
        update();

     if(draw)
        render();
    }

    terminate_game();
    return 0;
}
