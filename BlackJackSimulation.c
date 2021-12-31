#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/* initial condition */
int N = 15;     // 전체 플레이어 수
int p = 30;     // 카드를 뽑을 확률 [0 ~ 100(%)]
int limit = 17; // 딜러가 게임을 정지할 조건
int entire_deck[13] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};      // 뽑을 수 있는 카드의 종류

int rand_deck;  // 덱에서 어떤 카드를 뽑을지 결정하는 난수 (entire_deck과 연관있음)
int rand_draw;  // 카드를 뽑을지 말지 결정하는 난수 (p와 연관있음)
int drawn_card; // 뽑힌 카드 (entire_deck과 연관있음)

int win_ratio = 2;      // 승리할 경우 얻을 수 있는 금액의 배율
int betting_money = 1;  // 매판 베팅금액 1만원

void output_value(int **players_arr, int *count_arr)
/* 이중 포인터로 생성된 배열을 확인하는 함수 */
{
    for (int i = 0; i < N; i++) 
    {
        printf("%dth Player(%d) : [ ", i, count_arr[i]);
        for (int j = 0; j < count_arr[i]; j++)
        {
            printf("%d ", players_arr[i][j]);
        }
        printf("]\n");
    }
}

void draw_card(int **player_arr, int *count_arr, int *sum_arr, int index)
{
    rand_deck = rand()%13;                              // 어떤 카드를 뽑을지 (전역변수, 임시)
    drawn_card = entire_deck[rand_deck];                // 뽑은 카드 (전역변수, 임시)
    
    int individual_cardcount = count_arr[index];        // 현재 갖고있는 카드의 매수 (임시)
    int individual_sum = sum_arr[index];                // 현재 갖고있는 카드의 합 (임시)

    if (individual_sum + drawn_card > 21)   // 뽑은 카드 포함 21이 넘어갈경우
    {

        bool is_there_ace = false;      // ace가 있는지 없는지 확인하는 변수
        bool is_over_21 = true;        // 21이 넘어가면 죽어야하니까

        for(int j = 0; j < count_arr[index]; j++)
        {
            if(player_arr[index][j] == 11)  // 현재 덱에 A가 있을경우
            {
                player_arr[index][j] = 1;   // 그 A는 1로 변경
                sum_arr[index] -= 10;       // 따라서 Sum도 바뀜
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
            /* 21 넘겼으면 죽어야지 */

            /* 덱 초기화. 근데 굳이 그럴 필요 있나 싶어서 덱은 냅두기
            for (int j = 0; j < count_arr[index]; j++)
            {
                player_arr[index][j] = 0;
            }
            */
            count_arr[index] = 0;
            sum_arr[index] = 0;
            printf("%dth player is busted\n", index);     // 디버그용 프린트
            return;
        }
        
    }

    /* 뽑은 카드를 player_arr에 넣기위한 재할당 과정*/

    player_arr[index] = (int *)realloc(player_arr[index], sizeof(int) * (individual_cardcount+1));
    player_arr[index][individual_cardcount] = drawn_card;
    count_arr[index] += 1;
    sum_arr[index] += drawn_card;
}

void dealer_draw(int *dealer_arr, int *count, int *sum)
{
    rand_deck = rand()%13;                              // 어떤 카드를 뽑을지 (전역변수, 임시)
    drawn_card = entire_deck[rand_deck];                // 뽑은 카드 (전역변수, 임시)
    
    if (*sum + drawn_card > 21)   // 뽑은 카드 포함 21이 넘어갈경우
    {
        bool is_there_ace = false;      // ace가 있는지 없는지 확인하는 변수
        bool is_over_21 = true;        // 21이 넘어가면 죽어야하니까

        for(int j = 0; j < *count; j++)
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
            *count = 0;
            *sum = 0;
            printf("Dealer is busted\n");     // 디버그용 프린트
            return;
        }
        
    }

    /* 뽑은 카드를 dealer_arr에 넣기위한 재할당 과정*/

    dealer_arr = (int *)realloc(dealer_arr, sizeof(int) * (*count+1));
    dealer_arr[*count] = drawn_card;
    *count += 1;
    *sum += drawn_card;
}

void game_set(int sum_dealer, int *sum_player, int *money_player)
{
    for (int i = 0; i < N; i++)
    {
        // sum_dealer = 0 이면 dealer가 bust 한것.
        if (sum_player[i] > sum_dealer)
        {
            money_player[i] += betting_money * win_ratio;   // 이겼으니까 배당금
        }
        else if (sum_player[i] == sum_dealer)
        {
            money_player[i] += betting_money;   // 비겼으니까 원금회수
        }
        // 지면 돈 안줌
    }
}

void play_game(int *money_of_player)
{
    /* 동적할당 선언 */
    int *count_of_deck;
    count_of_deck = (int *)malloc(sizeof(int *) * N);     // 덱 숫자에 대한 정보가 담겨있는 arr
    for (int i = 0; i < N; i++)
    {
        count_of_deck[i] = 0;
    }

    int *sum_of_deck;
    sum_of_deck = (int *)malloc(sizeof(int *) * N);     // 카드의 합에 대한 정보가 담겨있는 arr
    for (int i = 0; i < N; i++)
    {
        sum_of_deck[i] = 0;
    }

    int **players;
    players = (int **)malloc(sizeof(int *) * N);  // 배열의 row. N명의 자리
    for (int i = 0; i < N; i++)
    {
        players[i] = (int *)malloc(sizeof(int) * 2);   // 배열의 col. 2장 먼저 받고 시작

        if (money_of_player[i] != 0)
        {
            money_of_player[i] -= betting_money;
            for (int j = 0; j < 2; j++)
            {
                rand_deck = rand()%13;                              // 어떤 카드를 뽑을지 (전역변수, 임시)
                drawn_card = entire_deck[rand_deck];                // 뽑은 카드 (전역변수, 임시)
                
                players[i][j] = drawn_card;                         // 카드 뽑았으니 덱에 넣기
                count_of_deck[i]++;                                 // 카드 매수 늘리기
                sum_of_deck[i] += drawn_card;                       // 카드의 합 늘리기

                if (sum_of_deck[i] > 21)
                {
                    // 첫 두번의 드로우에서 21이 넘는 경우는 A 두장, 즉 11 + 11 뿐이므로 나중에 뽑힌 11을 1로 만들어주면 된다.
                    players[i][j] = 1;      
                    sum_of_deck[i] -= 10;
                }
            }
        }
        
    }

    /* 딜러도 패는 나눠줘야지 */
    int *dealer;
    int dealer_count = 0;
    int dealer_sum = 0;

    dealer = (int *)malloc(sizeof(int *) * 2);

    for (int j = 0; j < 2; j++)
    {
        rand_deck = rand()%13;                              // 어떤 카드를 뽑을지 (전역변수, 임시)
        drawn_card = entire_deck[rand_deck];                // 뽑은 카드 (전역변수, 임시)
            
        dealer[j] = drawn_card;                         // 카드 뽑았으니 덱에 넣기
        dealer_count++;                                 // 카드 매수 늘리기
        dealer_sum += drawn_card;                       // 카드의 합 늘리기

        if (dealer_sum > 21)
        {
            // 첫 두번의 드로우에서 21이 넘는 경우는 A 두장, 즉 11 + 11 뿐이므로 나중에 뽑힌 11을 1로 만들어주면 된다.
            dealer[j] = 1;      
            dealer_sum -= 10;
        }
    }

    for (int i = 0; i < N; i++)     // N명의 모든 사람들에게 확인
    {
        bool is_draw_done = false;  // draw가 끝났는지 확인하는 변수
        while(!is_draw_done)
        {
            rand_draw = rand()%100;
            if (rand_draw <= p)     // 카드를 뽑겠다 판단
            {
                draw_card(players, count_of_deck, sum_of_deck, i);
            }
            else        // 카드를 뽑지 않겠다 판단
            {
                is_draw_done = true;
            }
        }
    }

    while(dealer_sum > 1 && dealer_sum < limit)
    {
        dealer_draw(dealer, &dealer_count, &dealer_sum);
    }
    game_set(dealer_sum, sum_of_deck, money_of_player);     // 게임 종료

    printf("======================= THIS IS FOR DEBUG =======================\n");
    printf("Dealer(%d) : [ ", dealer_sum);
    for (int j = 0; j < dealer_count; j++)
    {
        printf("%d ", dealer[j]);
    }
    printf("]\n");
    for (int i = 0; i < N; i++) 
    {
        printf("%dth player(%d) : [ ", i, money_of_player[i]);
        for (int j = 0; j < count_of_deck[i]; j++)
        {
            printf("%d ", players[i][j]);
        }
        printf("]\n");
        
    }
    printf("======================= THIS IS FOR DEBUG =======================\n");

    /* 동적할당 해제 */
    for (int i = 0; i < N; i++)
    {
        free(players[i]);
    }
    free(dealer);
    free(players);
    free(count_of_deck);
    free(sum_of_deck);
    
}

int main()
{
    srand(time(NULL)); // 난수 초기화

    int *money;
    money = (int *)malloc(sizeof(int *) * N);     // 카드의 합에 대한 정보가 담겨있는 arr
    for (int i = 0; i < N; i++)
    {
        money[i] = 10;     // 초기 금액 100만원
    }

    printf("START GAME\n\n\n\n\n\n\n\n\n\n\n");
    for (int game_number = 0; game_number < 100; game_number++)
    {
        printf("%dth game\n\n\n", game_number);
        play_game(money);
    }

    free(money);
}