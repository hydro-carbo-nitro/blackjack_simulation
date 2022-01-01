#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/* global variables */
int i, j;       // for loop를 돌리기 위한 임시 변수

int N = 20;     // 전체 플레이어 수
int total_games = 10;       // 몇판 게임할지
int p = 30;     // 카드를 뽑을 확률 [0 ~ 100(%)]
int limit = 17; // 딜러가 게임을 정지할 조건
int entire_deck[13] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};      // 뽑을 수 있는 카드의 종류

int rand_deck;  // 덱에서 어떤 카드를 뽑을지 결정하는 난수 (entire_deck과 연관있음)
int rand_draw;  // 카드를 뽑을지 말지 결정하는 난수 (p와 연관있음)
int drawn_card; // 뽑힌 카드 (entire_deck과 연관있음)

int win_ratio = 2;      // 승리할 경우 얻을 수 있는 금액의 배율
int betting_money = 1;  // 매판 베팅금액 1만원

bool is_draw_done;      // 드로우가 끝났는지 판단하는 boolean

void draw_card(int **player_arr, int *cnt_arr, int *sum_arr, int index, bool *judge);
void dealer_draw(int *dealer_arr, int *count, int *sum);
void play_game(int *money_of_player, int *total_money_of_player);
void game_set(int sum_dealer, int *sum_player, int *money_player, int *total_money);

int *create_1d_malloc(int size_of_array)    // 1dim 동적할당
{
    int *array;
    array = (int *)malloc(sizeof(int) * size_of_array);

    for (int i = 0; i < size_of_array; i++)
    {
        array[i] = 0;     // 초기값
    }

    return array;
}

int **create_2d_malloc(int row, int col)    // 2dim 동적할당
{
    int **array;
    array = (int **)malloc(sizeof(int *) * row);
    for (int i = 0; i < row; i++)
    {
        array[i] = (int *)malloc(sizeof(int) * col);
        for (int j = 0; j < col; j++)
        {
            array[i][j] = 0;     // 초기값
        }
    }

    return array;
}


int main()
{
    srand(time(NULL)); // 난수 초기화

    int *money = create_1d_malloc(N);                   // 각 플레이어들이 갖고있는 판돈
    int *total_money = create_1d_malloc(total_games);   // 매 라운드마다 누적판돈

    for (int game_cnt = 0; game_cnt < total_games; game_cnt++)
    {
        printf("==== ==== ==== ==== %dth game ==== ==== ==== ====\n", game_cnt);
        play_game(money, &total_money[game_cnt]);
    }

    free(money);
    free(total_money);
}

void play_game(int *money_of_player, int *total_money_of_player)
{
    // 변수 선언
    int *cnt_of_player = create_1d_malloc(N);   // 플레이어가 갖고있는 카드의 매수
    int *sum_of_player = create_1d_malloc(N);   // 플레이어가 갖고있는 카드의 총합
    int **player = create_2d_malloc(N, 2);      // 플레이어가 갖고있는 패의 정보. 나중에 realloc됨.
    int cnt_of_dealer = 0;                      // 딜러가 갖고있는 카드의 매수
    int sum_of_dealer = 0;                      // 딜러가 갖고있는 카드의 총합
    int *dealer = create_1d_malloc(2);          // 딜러가 갖고있는 패의 정보. 나중에 realloc됨.
    int total_money = 0;                        // 이번판의 변동 금액

    // 패 나눠주는 과정
    for (i = 0; i < N; i++)
    {
        money_of_player[i] -= betting_money;    // 매판 참가할때마다 판돈 걸기
        total_money -= betting_money;           // 총액도 변해야함
        
        // 첫 두장 나눠주는 과정
        for (j = 0; j < 2; j++)
        {
            rand_deck = rand()%13;                  // 어떤 카드를 뽑을지 (전역변수, 임시)
            drawn_card = entire_deck[rand_deck];    // 뽑은 카드 (전역변수, 임시)
                
            player[i][j] = drawn_card;              // 카드 뽑았으니 덱에 넣기
            cnt_of_player[i]++;                     // 카드 매수 늘리기
            sum_of_player[i] += drawn_card;         // 카드의 합 늘리기

            if (sum_of_player[i] > 21)
            {
                // 첫 두번의 드로우에서 21이 넘는 경우는 A 두장, 즉 11 + 11 뿐이므로 나중에 뽑힌 11을 1로 만들어주면 된다.
                player[i][j] = 1;      
                sum_of_player[i] -= 10;
            }
        }

        // 그다음 추가로 받을지 말지 결정
        is_draw_done = false;
        while(!is_draw_done)
        {
            rand_draw = rand()%100;
            if (rand_draw <= p)     // 카드를 뽑겠다 판단
            {
                draw_card(player, cnt_of_player, sum_of_player, i, &is_draw_done);
            }
            else                    // 카드를 뽑지 않겠다 판단
            {
                is_draw_done = true;
            }
        }

    }

    // 플레이어 차례 끝났으니 딜러 차례
    // 첫 두장 나눠주는 과정
    for (i = 0; i < 2; i++)
    {
        rand_deck = rand()%13;                              // 어떤 카드를 뽑을지 (전역변수, 임시)
        drawn_card = entire_deck[rand_deck];                // 뽑은 카드 (전역변수, 임시)
            
        dealer[i] = drawn_card;                         // 카드 뽑았으니 덱에 넣기
        cnt_of_dealer++;                                 // 카드 매수 늘리기
        sum_of_dealer += drawn_card;                       // 카드의 합 늘리기
        if (sum_of_dealer > 21)
        {
            // 첫 두번의 드로우에서 21이 넘는 경우는 A 두장, 즉 11 + 11 뿐이므로 나중에 뽑힌 11을 1로 만들어주면 된다.
            dealer[i] = 1;      
            sum_of_dealer -= 10;
        }
    }

    // 딜러가 파산하지 않았거나, limit를 넘기지 않았으면 계속 드로우를 해야함
    while(sum_of_dealer > 1 && sum_of_dealer < limit)
    {
        printf("\n");
        dealer_draw(dealer, &cnt_of_dealer, &sum_of_dealer);
    }

    // 딜러도 드로우가 끝났으니 승패 확인
    game_set(sum_of_dealer, sum_of_player, money_of_player, &total_money);
    *total_money_of_player = total_money;   // 이번판 금액 반영

    // 메모리 해제
    free(cnt_of_player);
    free(sum_of_player);
    free(dealer);
    for (i = 0; i < N; i++)
    {
        free(player[i]);
    }
    free(player);
}

void draw_card(int **player_arr, int *cnt_arr, int *sum_arr, int index, bool *judge)
{
    rand_deck = rand()%13;                              // 어떤 카드를 뽑을지 (전역변수, 임시)
    drawn_card = entire_deck[rand_deck];                // 뽑은 카드 (전역변수, 임시)
    
    int individual_cardcount = cnt_arr[index];          // 현재 갖고있는 카드의 매수 (임시)
    int individual_sum = sum_arr[index];                // 현재 갖고있는 카드의 합 (임시)

    if (individual_sum + drawn_card > 21)               // 뽑은 카드 포함 21이 넘어갈경우
    {

        bool is_there_ace = false;                      // ace가 있는지 없는지 확인하는 변수
        bool is_over_21 = true;                         // 21이 넘어가면 죽어야하니까

        for(int j = 0; j < cnt_arr[index]; j++)
        {
            if(player_arr[index][j] == 11)              // 현재 덱에 A가 있을경우
            {
                player_arr[index][j] = 1;               // 그 A는 1로 변경
                sum_arr[index] -= 10;                   // 따라서 Sum도 바뀜
                is_there_ace = true;
                is_over_21 = false;
            }
        }

        if(drawn_card == 11 && !is_there_ace)           // 덱에도 A, 손패도 A일 경우 하나만 1이 되어야하므로
        {
            // 손패는 A가 떴고, 갖고있는 덱에는 A가 없고, 이거때문에 21은 넘겨버렸고
            drawn_card = 1;                             // 그러면 손패가 1이 되어야지
            is_over_21 = false;
        }

        if(is_over_21)
        {
            cnt_arr[index] = 0;
            sum_arr[index] = -1;
            printf("%dth player is busted\n", index);   // 디버그용 프린트
            *judge = true;
            return;
        }
    }

    /* 뽑은 카드를 player_arr에 넣기위한 재할당 과정*/

    player_arr[index] = (int *)realloc(player_arr[index], sizeof(int) * (individual_cardcount+1));
    player_arr[index][individual_cardcount] = drawn_card;
    cnt_arr[index] += 1;
    sum_arr[index] += drawn_card;
}

void dealer_draw(int *dealer_arr, int *cnt, int *sum)
{
    rand_deck = rand()%13;                              // 어떤 카드를 뽑을지 (전역변수, 임시)
    drawn_card = entire_deck[rand_deck];                // 뽑은 카드 (전역변수, 임시)
    
    if (*sum + drawn_card > 21)   // 뽑은 카드 포함 21이 넘어갈경우
    {
        bool is_there_ace = false;      // ace가 있는지 없는지 확인하는 변수
        bool is_over_21 = true;        // 21이 넘어가면 죽어야하니까

        for(int j = 0; j < *cnt; j++)
        {
            if(dealer_arr[j] == 11)  // 현재 덱에 A가 있을경우
            {
                dealer_arr[j] = 1;   // 그 A는 1로 변경
                *sum -= 10;           // 따라서 Sum도 바뀜
                is_there_ace = true;
                is_over_21 = false;
            }
        }

        if(drawn_card == 11 && !is_there_ace)   // 덱에도 A, 손패도 A일 경우 하나만 1이 되어야하므로
        {
            /* 손패는 A가 떴고, 갖고있는 덱에는 A가 없고, 이거때문에 21은 넘겨버렸고 */
            drawn_card = 1;    // 그러면 손패가 1이 되어야지
            is_over_21 = false;
        }

        if(is_over_21)
        {
            *cnt = 0;
            *sum = 0;
            printf("Dealer is busted\n\n");     // 디버그용 프린트
            return;
        }    
    }

    /* 뽑은 카드를 dealer_arr에 넣기위한 재할당 과정*/

    dealer_arr = (int *)realloc(dealer_arr, sizeof(int) * (*cnt+1));
    dealer_arr[*cnt] = drawn_card;
    *cnt += 1;
    *sum += drawn_card;
}


void game_set(int sum_dealer, int *sum_player, int *money_player, int *total_money)
{
    for (int i = 0; i < N; i++)
    {
        // sum_dealer = 0 이면 dealer가 bust 한것.
        if (sum_player[i] > sum_dealer)
        {
            money_player[i] += betting_money * win_ratio;   // 이겼으니까 배당금
            *total_money += betting_money * win_ratio;      // 토탈에 반영
        }
        else if (sum_player[i] == sum_dealer)
        {
            money_player[i] += betting_money;   // 비겼으니까 원금회수
            *total_money += betting_money;      // 토탈에 반영
        }
        // 지면 돈 안줌
    }
}