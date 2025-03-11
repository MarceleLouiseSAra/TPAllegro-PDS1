#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <time.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

//gera um número aleatorio entre 0 e n-1
int random(int n) {
	return rand()%n;
}

const float FPS = 100;  
const int SCREEN_W = 960;
const int SCREEN_H = 540;
const int NAVE_W = 100;
int NAVE_H = 50;
const int RAIO_TIRO = 5;
const float ACEL_TIRO = 0.05;

void desenha_cenario(){
	ALLEGRO_COLOR cor = al_map_rgb(255,255,240);
	al_clear_to_color(cor);
}

typedef struct Vida{
	ALLEGRO_COLOR cor;
	int vidas;
	
}Vida;

typedef struct Tiro{
	float x, y;
	int ativo;
	int acertou;
	int hit;
	float vel_y;
	
}Tiro;

typedef struct Nave{
	float x,y;
	ALLEGRO_COLOR cor;
	int dir;
	float vel;
	Tiro tiro;
	
}Nave;

typedef struct Jogador{
	Nave nave;
	Vida vida;
	int pontuacao;
	
}Jogador;

void initJogador(Jogador *j1, Jogador *j2){
	int vel = 1 + random(3);
	
	//Cria a Nave 1:
	(*j1).nave.x = -1;
	(*j1).nave.y = NAVE_H + 30;
	(*j1).nave.cor = al_map_rgb(222,49,99);
	(*j1).nave.dir = 1;
	(*j1).nave.vel = vel;
	(*j1).nave.tiro.x = 0;
	(*j1).nave.tiro.y = 0;
	(*j1).nave.tiro.ativo = 0;
	(*j1).nave.tiro.hit = 0;
	(*j1).nave.tiro.vel_y = 0;
	
	(*j1).vida.vidas = 3;
	(*j1).vida.cor = (*j1).nave.cor;
	(*j1).pontuacao = 0;
	
	//Cria a Nave 2:
	(*j2).nave.x = SCREEN_W;
	(*j2).nave.y = NAVE_H + 110;
	(*j2).nave.cor = al_map_rgb(65,105,225);
	(*j2).nave.dir = -1;
	(*j2).nave.vel = vel;
	(*j2).nave.tiro.x = 0;
	(*j2).nave.tiro.y = 0;
	(*j2).nave.tiro.ativo = 0;
	(*j2).nave.tiro.hit = 0;
	(*j2).nave.tiro.vel_y = 0;
	
	(*j2).vida.vidas = 3;
	(*j2).vida.cor = (*j2).nave.cor;
	(*j2).pontuacao = 0;
}

void desenha_nave(Jogador *j){
	//desenha a nave:
	
	al_draw_filled_triangle((*j).nave.x, (*j).nave.y,
							(*j).nave.x - ((*j).nave.dir*NAVE_W),
							(*j).nave.y - NAVE_H/2,
							(*j).nave.x - ((*j).nave.dir*NAVE_W),
							(*j).nave.y + NAVE_H/2,
							(*j).nave.cor);
	//desenha o tiro:
	if((*j).nave.tiro.ativo == 1){
		al_draw_filled_circle((*j).nave.tiro.x,
						  (*j).nave.tiro.y,
						  RAIO_TIRO, (*j).nave.cor);
	}
}

void atualiza_naves(Jogador *j1, Jogador *j2){
	Jogador aux;
	(*j1).nave.x += ((*j1).nave.dir)*(*j1).nave.vel;
	(*j2).nave.x += ((*j2).nave.dir)*(*j2).nave.vel;
	if(((*j1).nave.x > SCREEN_W + NAVE_W || (*j1).nave.x < -NAVE_W) || 
		((*j2).nave.x > SCREEN_W + NAVE_W || (*j2).nave.x < -NAVE_W)){
		(*j1).nave.dir *= -1;
		(*j2).nave.dir *= -1;
		(*j1).nave.vel = (*j2).nave.vel = 1 + random(3);
		desenha_nave(j1);
		desenha_nave(j2);
		if(random(2)){
			aux.nave.y = (*j1).nave.y;
			(*j1).nave.y = (*j2).nave.y;
			(*j2).nave.y = aux.nave.y;
		}
		
		if(random(2)){
			NAVE_H = 15 + random(31);
		}
	}
}

void atualiza_tiro(Jogador *j){
	if((*j).nave.tiro.ativo) {
		(*j).nave.tiro.x = (*j).nave.x;
		(*j).nave.tiro.vel_y += ACEL_TIRO;
		(*j).nave.tiro.y += (*j).nave.tiro.vel_y;
	}
	
	if((*j).nave.tiro.x < -RAIO_TIRO ||
	   (*j).nave.tiro.x > SCREEN_W + RAIO_TIRO ||
	   (*j).nave.tiro.y > SCREEN_H + RAIO_TIRO) {
		   (*j).nave.tiro.ativo = 0;
	}	
}

void vidas(Jogador *j1, Jogador *j2, Vida v1[3], Vida v2[3]){
	
	al_draw_filled_circle(85, 20, 8, v1[2].cor);
	al_draw_filled_circle(55, 20, 8, v1[1].cor);
	al_draw_filled_circle(25, 20, 8, v1[0].cor);
	
	if((*j1).vida.vidas == 2){
		v1[2].cor = al_map_rgb(255,255,240);
	}else{
		v1[2].cor = (*j1).nave.cor;
	}
	if((*j1).vida.vidas == 1){
		v1[2].cor = al_map_rgb(255,255,240);
		v1[1].cor = al_map_rgb(255,255,240);
	}else{
		v1[1].cor = (*j1).nave.cor;
	}
	if((*j1).vida.vidas == 0){
		v1[2].cor = al_map_rgb(255,255,240);
		v1[1].cor = al_map_rgb(255,255,240);
		v1[0].cor = al_map_rgb(255,255,240);
	}else{
		v1[0].cor = (*j1).nave.cor;
	}
	
	al_draw_filled_circle(875, 20, 8, v2[2].cor);
	al_draw_filled_circle(905, 20, 8, v2[1].cor);
	al_draw_filled_circle(935, 20, 8, v2[0].cor);
	
	if((*j2).vida.vidas == 2){
		v2[2].cor = al_map_rgb(255,255,240);
	}else{
		v2[2].cor = (*j2).nave.cor;
	}
	if((*j2).vida.vidas == 1){
		v2[2].cor = al_map_rgb(255,255,240);
		v2[1].cor = al_map_rgb(255,255,240);
	}else{
		v2[1].cor = (*j2).nave.cor;
	}
	if((*j2).vida.vidas == 0){
		v2[2].cor = al_map_rgb(255,255,240);
		v2[1].cor = al_map_rgb(255,255,240);
		v2[0].cor = al_map_rgb(255,255,240);
	}else{
		v2[0].cor = (*j2).nave.cor;
	}

}

#define TAM_Y 380
#define TAM_X 960
#define LINHAS 8
#define COLUNAS 20 

typedef struct Tile{
	int ativo;
	int premio;
	ALLEGRO_COLOR cor;
	int x1, y1, x2, y2;
	
}Tile;

void criaGrid(Tile g[]){
	int i, tam_x = 0, tam_y = 380;
	for(i=0; i<20; i++){
		g[i].cor = al_map_rgb(222,49,99);
		g[i].premio = 1;
		g[i].ativo = 1;
		g[i].x1 = tam_x;
		g[i].y1 = tam_y;
		tam_x += 48;
		g[i].x2 = tam_x;
		g[i].y2 = tam_y + 20;
	}
	
	tam_x = 0;
	tam_y = 400;
	for(i=20; i<40; i++){
		g[i].cor = al_map_rgb(255,105,180);
		g[i].premio = 2;
		g[i].ativo = 1;
		g[i].x1 = tam_x;
		g[i].y1 = tam_y;
		tam_x += 48;
		g[i].x2 = tam_x;
		g[i].y2 = tam_y + 20;
	}
	
	tam_x = 0;
	tam_y = 420;
	for(i=40; i<60; i++){
		g[i].cor = al_map_rgb(238,130,238);
		g[i].premio = 3;
		g[i].ativo = 1;
		g[i].x1 = tam_x;
		g[i].y1 = tam_y;
		tam_x += 48;
		g[i].x2 = tam_x;
		g[i].y2 = tam_y + 20;
	}
	
	tam_x = 0;
	tam_y = 440;
	for(i=60; i<80; i++){
		g[i].cor = al_map_rgb(218,112,214);
		g[i].premio = 4;
		g[i].ativo = 1;
		g[i].x1 = tam_x;
		g[i].y1 = tam_y;
		tam_x += 48;
		g[i].x2 = tam_x;
		g[i].y2 = tam_y + 20;
	}
	
	tam_x = 0;
	tam_y = 460;
	for(i=80; i<100; i++){
		g[i].cor = al_map_rgb(186,85,211);
		g[i].premio = 5;
		g[i].ativo = 1;
		g[i].x1 = tam_x;
		g[i].y1 = tam_y;
		tam_x += 48;
		g[i].x2 = tam_x;
		g[i].y2 = tam_y + 20;
	}
	
	tam_x = 0;
	tam_y = 480;
	
	for(i=100; i<120; i++){
		g[i].cor = al_map_rgb(123,104,238);
		g[i].premio = 6;
		g[i].ativo = 1;
		g[i].x1 = tam_x;
		g[i].y1 = tam_y;
		tam_x += 48;
		g[i].x2 = tam_x;
		g[i].y2 = tam_y + 20;
	}
	
	tam_x = 0;
	tam_y = 500;
	for(i=120; i<140; i++){
		g[i].cor = al_map_rgb(65,105,225);
		g[i].premio = 7;
		g[i].ativo = 1;
		g[i].x1 = tam_x;
		g[i].y1 = tam_y;
		tam_x += 48;
		g[i].x2 = tam_x;
		g[i].y2 = tam_y + 20;
	}
	
	tam_x = 0;
	tam_y = 520;
	for(i=140; i<160; i++){
		g[i].cor = al_map_rgb(58,28,255);
		g[i].premio = 8;
		g[i].ativo = 1;
		g[i].x1 = tam_x;
		g[i].y1 = tam_y;
		tam_x += 48;
		g[i].x2 = tam_x;
		g[i].y2 = tam_y + 20;
	}
}

void desenhaGrid(Tile g[]){
	int i;
	for(i=0; i<160; i++){
		if(g[i].ativo == 1){
			al_draw_filled_rectangle(g[i].x1, g[i].y1, g[i].x2, g[i].y2, g[i].cor);
		}
	}
}

void jogada(Tile g[], Jogador *j1, Jogador *j2){
	int i;
	for(i=0; i<COLUNAS*LINHAS; i++){
		if(g[i].ativo == 1
		&& (*j1).nave.tiro.y + RAIO_TIRO > g[i].y1 
		&& (*j1).nave.tiro.y + RAIO_TIRO < g[i].y2
		&& (*j1).nave.tiro.x > g[i].x1
		&& (*j1).nave.tiro.x < g[i].x2){
			//al_play_sample(colisao, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			(*j1).pontuacao += g[i].premio;
			(*j1).nave.tiro.acertou = 1;
			g[i].premio = 0;
			g[i].ativo = 0;
		}
	}
	
	if((*j1).nave.tiro.ativo == 1 && (*j1).nave.tiro.acertou == 0 && (*j1).nave.tiro.y + RAIO_TIRO > SCREEN_H){
		(*j1).nave.tiro.ativo = 0;
		(*j1).vida.vidas--;
	}

	for(i=0; i<COLUNAS*LINHAS; i++){
		if(g[i].ativo == 1
		&& (*j2).nave.tiro.y + RAIO_TIRO > g[i].y1 
		&& (*j2).nave.tiro.y + RAIO_TIRO < g[i].y2 
		&& (*j2).nave.tiro.x > g[i].x1
		&& (*j2).nave.tiro.x < g[i].x2){
			//al_play_sample(colisao, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			(*j2).pontuacao += g[i].premio;
			(*j2).nave.tiro.acertou = 1;
			g[i].premio = 0;
			g[i].ativo = 0;
		}
	}
	
	if((*j2).nave.tiro.ativo == 1 && (*j2).nave.tiro.acertou == 0 && (*j2).nave.tiro.y + RAIO_TIRO > SCREEN_H){
		(*j2).nave.tiro.ativo = 0;
		(*j2).vida.vidas--;
	}
	
}

int main(int argc, char **argv){
	
	srand(time(NULL));
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
   
	//----------------------- rotinas de inicializacao ---------------------------------------
    
	//inicializa o Allegro
	if(!al_init()){
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	
	
	//inicializa o modulo que permite carregar imagens no jogo
	if(!al_init_image_addon()){
		fprintf(stderr, "failed to initialize image module!\n");
		return -1;
	}
   
	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer){
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
 
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display){
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//instala o teclado
	if(!al_install_keyboard()){
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}
	
	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon()){
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
	
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);   
	if(size_32 == NULL){
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}

 	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue){
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
   
	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	
	Jogador j1, j2;
	initJogador(&j1, &j2);
	Vida v1[3], v2[3];
	
	Tile g[COLUNAS*8];
	criaGrid(g);
	
	/*ALLEGRO_SAMPLE *tiro = NULL; 
	ALLEGRO_SAMPLE *colisao = NULL; 
	ALLEGRO_SAMPLE *game_over = NULL; 
	ALLEGRO_SAMPLE *musica = NULL; 
	ALLEGRO_SAMPLE_INSTANCE *song_instance = NULL;
	
	al_reserve_samples(10);
	tiro = al_load_sample("");
	colisao = al_load_sample("");
	musica = al_load_sample("");
	game_over = al_load_sample("");
	song_instance = al_create_song_instance(musica);
	
	al_set_sample_instance_playmode(song_instance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(song_instance, al_get_default_mixer);
	al_play_sample_instance(song_instance);*/
	
	al_init_font_addon();
	al_init_ttf_addon();
	ALLEGRO_FONT *pontuacao = al_load_font("arial.ttf", 20, 0);
	char pontuacao1[100], pontuacao2[100];
	
	int i, aux = 0;
	
	//inicia o temporizador
	al_start_timer(timer);
	
	int playing = 1;
	while(playing){
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER){

			desenha_cenario();
			desenhaGrid(g);
			
			atualiza_tiro(&j1);
			desenha_nave(&j1);
			atualiza_tiro(&j2);
			desenha_nave(&j2);
			atualiza_naves(&j1, &j2);
			vidas(&j1, &j2, v1, v2);
			
			jogada(g, &j1, &j2);
			
			sprintf(pontuacao1, "%d", j1.pontuacao);
			al_draw_text(pontuacao, j1.nave.cor, 19, 32, 0, pontuacao1);
			sprintf(pontuacao2, "%d", j2.pontuacao);
			al_draw_text(pontuacao, j2.nave.cor, 870, 32, 0, pontuacao2);

			//atualiza a tela (quando houver algo para mostrar)
			al_flip_display();
			
			if(al_get_timer_count(timer)%(int)FPS == 0){
				printf("\n%d segundos se passaram...", (int)(al_get_timer_count(timer)/FPS));
			}
			
		}
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			playing = 0;
		}
		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
			
			switch(ev.keyboard.keycode){
				
				case ALLEGRO_KEY_SPACE:
					//al_play_sample(tiro, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					j1.nave.tiro.ativo = 1;
					j1.nave.tiro.acertou = 0;
					j1.nave.tiro.hit = 0;
					j1.nave.tiro.x = j1.nave.x;
					j1.nave.tiro.y = j1.nave.y;
					j1.nave.tiro.vel_y = 0;
					
				break;

				case ALLEGRO_KEY_ENTER:
					//al_play_sample(tiro, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					j2.nave.tiro.ativo = 1;
					j2.nave.tiro.acertou = 0;
					j2.nave.tiro.hit = 0;
					j2.nave.tiro.x = j2.nave.x;
					j2.nave.tiro.y = j2.nave.y;
					j2.nave.tiro.vel_y = 0;
					
				break;
			}
			
			//imprime qual tecla foi
			printf("\ncodigo tecla: %d", ev.keyboard.keycode);
		}
		
		aux = 0;
		for(i=0; i<160; i++){
			if(g[i].ativo == 1){
				aux++;
			}
		}
	
		if(aux == 0){
			//al_play_sample(game_over, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			break;
		}
	
		if((j1).vida.vidas == 0 || (j2).vida.vidas == 0){
			//al_play_sample(game_over, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			break;
		}
		
		
	}//fim do while
	
	FILE *arq;	
	int vict1 = 0, vict2 = 0;
	arq = fopen("pontuacao.dat", "r");
	fscanf(arq, "%d %d", &vict1, &vict2);
	
	if(j1.pontuacao > j2.pontuacao){
		vict1++;
	}else if(j2.pontuacao > j1.pontuacao){
		vict2++;
	}
	
	char svict1[4], svict2[4];
	sprintf(svict1, "%d", vict1);
	al_draw_text(pontuacao, al_map_rgb(222,49,99), 250, 200, 0, "Vitorias do jogador 1:");
	al_draw_text(pontuacao, al_map_rgb(222,49,99), 450, 200, 0, svict1);
	sprintf(svict2, "%d", vict2);
	al_draw_text(pontuacao, al_map_rgb(65,105,225), 500, 200, 0, "Vitorias do jogador 2:");
	al_draw_text(pontuacao, al_map_rgb(65,105,225), 700, 200, 0, svict2);
	al_flip_display();
	al_rest(4.0);
	
	arq = fopen("pontuacao.dat", "w");
	fprintf(arq, "%d %d", vict1, vict2);
	
	fclose(arq);
	
	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc):
	
	al_destroy_font(pontuacao);

	/*
	al_stop_sample_instance(song_instance);
	al_destroy_sample(tiro);
	al_destroy_sample(colisao);
	al_destroy_sample(game_over);
	al_destroy_sample(musica);
	al_destroy_sample(song_instance);
	*/

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}