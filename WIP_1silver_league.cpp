#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define TRAINING 0
#define CODING 1
#define DAILY_ROUTINE 2
#define TASK_PRIORITIZATION 3
#define ARCHITECTURE_STUDY 4
#define CONTINUOUS_INTEGRATION 5
#define CODE_REVIEW 6
#define REFACTORING 7

#define BONUS 8
#define DEBT 9

#define WAIT 0

using namespace std;

class   Data
{
    private:

    public:
        Data();
        ~Data();

        int current_location;
        int nb_release;
        int daily_routine_cards;
        int architecture_study_cards;
};

class   Application
{
    private:

    public:
        Application();
        ~Application();
        
        string  app_name;
        int     id;
        int     need[8];
};

class   Player
{
    private:

    public:
        Player();
        ~Player();

        int location;
        int score;
        int permanent_daily_routine_cards;
        int permanent_architecture_study_cards;
};

class   Deck
{
    private:

    public:
        Deck();
        ~Deck();

        string  deck_name;
        int     cards_count[10];
};

Application::Application(){}
Application::~Application(){}

Player::Player(){}
Player::~Player(){}

Deck::Deck(){}
Deck::~Deck(){}

Data::Data() : current_location(-1), nb_release(0), daily_routine_cards(0), architecture_study_cards(0) {}
Data::~Data(){}

void    print_remaining_location_cards(int *location_cards);
void    print_price(Application *apps, int *ressources, int app_count);


//         //TESTS//////////////////////////////////////////////////////////////////////////////////
//         cerr << game_phase << endl;
//         for (int i = 0; i < possible_moves_count; i++)
//             cerr << possible_move[i] << endl;
//         cerr << endl << endl << endl;
//         for (int i = 0; i < app_count; i++)
//             cerr << "TRAINING " << apps[i].need[0] << ", CODING " << apps[i].need[1] << ", DAILY_ROUTINE " << apps[i].need[2] << ", TASK_PRIORITIZATION " << apps[i].need[3] << ", ARCHITECTURE_STUDY " << apps[i].need[4] << ", CONITNUOUS_INTEGRATION " << apps[i].need[5] << ", CODE_REVIEW " << apps[i].need[6] << ", REFACTORING " << apps[i].need[7] << endl;;
//     }
// }


/////////////      UTILS         /////////////
bool    is_in_hand(vector<Deck> decks, int deck_count, int card_type)
{
    for (int i = 0; i < deck_count; i++)
    {
        if (decks[i].deck_name == "HAND")
        {
            if (decks[i].cards_count[card_type] > 0)
                return (true);
        }
    }
    return (false);
}

bool    is_useful_card(const vector<Application>& apps, const vector<Deck>& decks, int card)
{
    int needed[8] = {0, 0 ,0 ,0 ,0 ,0 ,0 ,0};
    int hand;
    int draw;
    int discard;
    int first = 1000;
    int second = 1000;
    int score = -1;

    for (int i = 0; i < apps.size(); i++)
    {
        for (int j = 0; j < 8; j++)
            needed[i] += apps[i].need[j];
    }
    for (int i = 0; i < decks.size(); i++)
    {
        if (decks[i].deck_name == "HAND")
            hand = i;
        else if (decks[i].deck_name == "DRAW")
            draw = i;
        else if (decks[i].deck_name == "DISCARD")
            discard = i;
    }
    for (int i = 0; i < 8; i++)
    {
        needed[i] -= decks[hand].cards_count[i];
        needed[i] -= decks[draw].cards_count[i];
        needed[i] -= decks[discard].cards_count[i];
    }
    for (int i = 0; i < 8; i++)
    {
        if (needed[i] > score)
        {
            score = needed[i];
            first = i;
        }
    }
    score = -1;
    for (int i = 0; i < 8; i++)
    {
        if (i != first && needed[i] > score)
        {
            score = needed[i];
            second = i;
        }
    }
    if (card == first || card == second)
        return (true);
    else
        return (false);
}

/////////////     FUNCTIONS      /////////////
int get_price(const int *app_need, const int *ressources)
{
    int price = 0;

    for (int i = 0; i < 8; i++)
    {
        if (app_need[i] > ressources[i])
            price += app_need[i] - ressources[i] * 2;
    }
    price -= ressources[BONUS];
    return (price);
}

bool    valid_release(const vector<string>& possible_moves, int id)
{
    for (int i = 0; i < possible_moves.size(); i++)
    {
        if (possible_moves[i] == "RELEASE " + to_string(id))
            return (true);
    }
    return (false);
}

int best_possible_release(const vector<Application>& apps, const vector<Deck>& decks, const vector<string>& possible_moves, int app_count, int deck_count, int tolerated_debt)
{
    int my_deck;
    int price;
    int best_price = 1000;
    int id_app = WAIT;

    for (int i = 0; i < deck_count; i++)
    {
        if (decks[i].deck_name == "HAND")
            my_deck = i;
    }
    for (int i = 0; i < app_count; i++)
    {
        price = get_price(apps[i].need, decks[my_deck].cards_count);
        if (price <= tolerated_debt && price < best_price && valid_release(possible_moves, apps[i].id))
        {
            best_price = price;
            id_app = apps[i].id;
        }
    }
    return (id_app);
}

void    get_remaining_location_cards(int *location_cards, vector<Deck> decks, int deck_count)
{
    for (int i = 0; i < deck_count; i++)
    {
        for (int j = 0; j < 8; j++)
            location_cards[j] -= decks[i].cards_count[j];
    }
}

bool    opponent_is_near(Data data, Player *player, int location)
{
    int opponent;

    if (player[0].location == data.current_location)
        opponent = 1;
    else
        opponent = 0;
    if (player[opponent].location == 0)
    {
        if (location == 8 || location == 0 || location == 1)
            return (true);
    }
    else if (player[opponent].location == 8)
    {
        if (location == 7 || location == 8 || location == 0)
            return (true);
    }
    else
    {
        if (location == player[opponent].location - 1 || location == player[opponent].location || location == player[opponent].location + 1)
            return (true);
    }
    return (false);
}

bool    no_special_cards(int *location_cards)
{
    int empty_location = 0;

    for (int i = 0; i < 8; i++)
    {
        if (location_cards[i] == 0)
            empty_location++;
    }
    if (empty_location == 0)
        return (true);
    else
        return (false);
}

////.   TAKE NEXT MOVE THAT IS NOT A BAD ONE 
int get_best_move(const vector<Application>& apps, int app_count, const vector<Deck>& decks, int deck_count, int *location_cards, Data data, Player *player)
{
    int needed[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int move = -1;
    int biggest_need = 0;

    for (int i = 0; i < app_count; i++)
    {
        for (int j = 0; j < 8; j++)
            needed[j] += apps[i].need[j];
    }
    if (no_special_cards(location_cards))
    {
        for (int i = data.current_location; i <= 8; i++)
        {
            if (i == 8)
                i = -1;
            else
            {
                if (!opponent_is_near(data, player, i) && data.current_location != i)
                {
                    move = i;
                    break ;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            if (data.current_location != i && needed[i] > biggest_need && location_cards[i] > 0 && !is_in_hand(decks, deck_count, i) && !opponent_is_near(data, player, i))
            {
                biggest_need = needed[i];
                move = i;
            }
        }
        if (move == -1 )
        {
            for (int i = 0; i < 8; i++)
            {
                if (data.current_location != i && !opponent_is_near(data, player, i))
                    move = i;
            }
        }
    }
    return (move);
}

int least_wanted_card(const vector<Application>& apps, const vector<Deck>& decks, const int *my_cards)
{
    int card;
    int needed[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int least_value = 100000;

    for (int i = 0; i < apps.size(); i++)
    {
        for (int j = 0; j < 8; j++)
            needed[j] += apps[i].need[j];
    }
    for (int i = 0; i < 8; i++)
    {
        if (needed[i] <= least_value && is_in_hand(decks, decks.size(), i)) 
        {
            least_value = needed[i];
            card = i;
        }
    }
    return (card);
}

int best_to_give(const vector<Application>& apps, const vector<Deck>& decks, int deck_count)
{
    int special_card = 0;

    for (int i = 0; i < 8; i++)
    {
        if (is_in_hand(decks, deck_count, i))
            special_card = 1;
    }
    if (special_card == 0)
        return (-1);
    else
    {
        int is_me = 1;

        for (int i = 0; i < decks.size(); i++)
        {
            if (decks[i].deck_name == "HAND")
                is_me = i;
        }
        return (least_wanted_card(apps, decks, decks[is_me].cards_count));
    }
}

int get_best_card(const vector<Application>& apps, const vector<Deck>& decks)
{
    int best_card;
    int biggest_score = -1;
    int draw = -1;
    int automated = -1;
    int needed[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < decks.size(); i++)
    {
        if (decks[i].deck_name == "DRAW")
            draw = i;
        else if (decks[i].deck_name == "AUTOMATED")
            automated = i;
    }
    cerr << draw << endl;
    cerr << automated << endl;
    for (int i = 0; i < apps.size(); i++)
    {
        for (int j = 0; j < 8; j++)
            needed[j] += apps[i].need[j];
    }
    if (draw != -1)
    {
        for (int i = 0; i < 8; i++)
            needed[i] = needed[i] - decks[draw].cards_count[i];
    }
    if (automated != -1)
    {
        for (int i = 0; i < 8; i++)
            needed[i] = needed[i] - decks[automated].cards_count[i];
    }
    for (int i = 0; i < 8; i++)
    {
        if (needed[i] > biggest_score)
        {
            biggest_score = needed[i];
            best_card = i;
        }
    }
    return (best_card);
}

int main()
{
    Data    data;
    while (1)
    {
        vector<Application> apps;
        Player              player[2];
        vector<Deck>        decks;
        vector<string>      possible_moves;

        string              game_phase;
        int                 app_count;
        int                 deck_count;
        int                 possible_moves_count;

        int                 location_cards[8] = {5, 5, 5, 5, 5, 5, 5, 5};
        vector<int>         apps_rank;

        //////////////////////////        INITIALIZATION         //////////////////////////////////

        cin >> game_phase; cin.ignore();

        //INIT APPS
        cin >> app_count; cin.ignore();
        for (int i = 0; i < app_count; i++)
        {
            Application tmp_app;

            cin >> tmp_app.app_name;
            cin >> tmp_app.id;
            for (int j = 0; j < 8; j++)
                cin >> tmp_app.need[j];
            cin.ignore();
            apps.push_back(tmp_app);
        }

        //INIT PLAYER
        for (int i = 0; i < 2; i++)
            cin >> player[i].location >> player[i].score >> player[i].permanent_daily_routine_cards >> player[i].permanent_architecture_study_cards; cin.ignore();

        //INIT DECKS
        cin >> deck_count; cin.ignore();
        for (int i = 0; i < deck_count; i++)
        {
            Deck    tmp_deck;

            cin >> tmp_deck.deck_name;
            for (int j = 0; j < 10; j++)
                cin >> tmp_deck.cards_count[j];
            cin.ignore();
            decks.push_back(tmp_deck);
        }

        //INIT MOVES
        cin >> possible_moves_count; cin.ignore();
        for (int i = 0; i < possible_moves_count; i++)
        {
            string  tmp_move;

            getline(cin, tmp_move);
            possible_moves.push_back(tmp_move);
        }

        ///////////////////////      GET INTEL       //////////////////////////

        get_remaining_location_cards(location_cards, decks, deck_count);
                         //print_remaining_location_cards(location_cards);

        ///////////////////////      EXECUTION       //////////////////////////

        //MOVE
        if (game_phase == "MOVE")
        {
            int move;

            move = get_best_move(apps, app_count, decks, deck_count, location_cards, data, player);
            data.current_location = move;
            cout << "MOVE " << move << endl;

        }
        //GIVE
        else if (game_phase == "GIVE_CARD")
        {
            int card = -1;

            card = best_to_give(apps, decks, deck_count);
            if (is_in_hand(decks, deck_count, BONUS))
                cout << "GIVE " << BONUS << endl;
            else if (card == -1)
                cout << "RANDOM" << endl;
            else
                cout << "GIVE " << card << endl;
        }
        //THROW
        else if (game_phase == "THROW_CARD")
        {
            int card = -1;

            card = best_to_give(apps, decks, deck_count);
            if (is_in_hand(decks, deck_count, BONUS))
                cout << "THROW " << BONUS << endl;
            else if (card == -1)
                cout << "RANDOM" << endl;
            else
                cout << "THROW " << card << endl;
        }
        //PLAY
        else if (game_phase == "PLAY_CARD")
        {
            int best_card = get_best_card(apps, decks);
            if (data.nb_release < 4)
            {
                //CONTINUOUS_INTEGRATION
                if (is_in_hand(decks, deck_count, CONTINUOUS_INTEGRATION) && is_in_hand(decks, deck_count, BONUS) && best_possible_release(apps, decks, possible_moves, app_count, deck_count, 0) == 0)
                    cout << "CONTINUOUS_INTEGRATION " << BONUS << endl;
                //TRAINING
                else if (best_card != TRAINING && is_in_hand(decks, deck_count, TRAINING) && best_possible_release(apps, decks, possible_moves, app_count, deck_count, 0) == 0)
                    cout << "TRAINING" << endl;
                else
                    cout << "WAIT" << endl;
            }
            else
            {
                for (int i = 0; i < possible_moves_count; i++)
                    cerr << possible_moves[i] << endl;
                cerr << "BEST_CARD " << best_card << endl;
                //TRAINING
                if (best_card != TRAINING && is_in_hand(decks, deck_count, TRAINING) && best_possible_release(apps, decks, possible_moves, app_count, deck_count, 0) == 0)
                    cout << "TRAINING" << endl;
                //BEST_CARD_CONTINUOUS_INTEGRATION
                else if (best_card != CONTINUOUS_INTEGRATION && is_in_hand(decks, deck_count, CONTINUOUS_INTEGRATION) && is_in_hand(decks, deck_count, best_card) && best_possible_release(apps, decks, possible_moves, app_count, deck_count, 0) == 0)
                    cout << "CONTINUOUS_INTEGRATION " << best_card << endl;
                //CONTINUOUS_INTEGRATION
                else if (is_in_hand(decks, deck_count, CONTINUOUS_INTEGRATION) && is_in_hand(decks, deck_count, BONUS) && best_possible_release(apps, decks, possible_moves, app_count, deck_count, 0) == 0)
                    cout << "CONTINUOUS_INTEGRATION " << BONUS << endl;
                //ARCHITECTURE_STUDY
                else if (is_in_hand(decks,deck_count, ARCHITECTURE_STUDY) && best_possible_release(apps, decks, possible_moves, app_count, deck_count, 0) == 0)
                    cout << "ARCHITECTURE_STUDY" << endl;
                //REFACTORING
                else if (best_card != REFACTORING && is_in_hand(decks, deck_count, REFACTORING) && is_in_hand(decks, deck_count, DEBT) && best_possible_release(apps, decks, possible_moves, app_count, deck_count, 0) == 0)
                {
                    cout << "REFACTORING" << endl;
                    cerr << "/////////////////////////////////////////////////////////////////////" << endl;
                }
                else
                    cout << "WAIT" << endl;
            }
        }
        //RELEASE
        else if (game_phase == "RELEASE")
        {
            int release = -1;

            if (release < 4)
                release = best_possible_release(apps, decks, possible_moves, app_count, deck_count, 0);
            else
                release = best_possible_release(apps, decks, possible_moves, app_count, deck_count, 0);
            if (release != WAIT)
            {
                cout << "RELEASE " << release << endl;
                data.nb_release++;
            }
            else
                cout << "WAIT" << endl;
        }
        //UNEXPECTED CASES
    }
}

/////////////     TESTEURS       /////////////
// void    print_remaining_location_cards(int *location_cards)
// {
//     for (int i = 0; i < 8; i++)
//         cerr << i << ":  " << location_cards[i] << endl;
// }

// void    print_price(Application *apps, int *ressources, int app_count)
// {
//     for (int i = 0; i < app_count; i++)
//         cerr << "Index: " << i << " / Price: " << get_price(apps[i].need, ressources) << endl;
//     cerr << endl << endl << endl;
// }
