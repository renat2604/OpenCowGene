//////////// LIBS ////////////

#include <stdio.h>
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <strstream>
#include <iomanip>
#include <chrono>

//////////// DEFINES ////////////



using namespace std;

condition_variable initializationPrintCond, initializationHandlerSemaphoreCond, initializationHandlerCowCond;
condition_variable processPrintCond, processHandlerSemaphoreCond, processHandlerCowCond, processPrintCowCond;
condition_variable processFileAllInfoPrintCond, processHandlerSemaphoreInfoCond;
mutex initializationPrintMutex, initializationHandlerMutex, initializationGeneralCharsHandlerMutex, initializationCowPreprocessorMutex, initializationCowSemaphoreMutex;
mutex processPrintMutex, processOutputWorldHandlerMutex, processOutputExistCowHandlerMutex, processOutputNotExistCowHandlerMutex, processGeneralCharsHandlerMutex, processCowPreprocessorMutex, processCowSemaphoreMutex, processCowHandlerMutex, processWorldHandlerMutex, cowPoolMoverMutex, cowPoolFillerMutex, cowHandlerBoolMutex, processOutputMutex;
mutex processFileAllInfoMutex, processGeneralCharsHandlerInfoMutex, processFileMainAllInfoMutex;
bool initializationPrintBool = false;
bool processPrintBool = false, processCowPrintSemaphoreContinue = false;
bool processFileAllInfoPrintBool = false;
unsigned __int8 cowHandlerBool = 0b00000000;
unsigned __int16 initializationHandlerSemaphore = 0x001F; unsigned __int8 initializationCowPrintSemaphoreStart = 0;
unsigned __int16 processHandlerSemaphore = 0x001F; unsigned __int8 processCowPrintSemaphoreStart = 0;
unsigned __int16 processHandlerInfoSemaphore = 0x00FF;

//////////// TEMPLATES ////////////

template <typename T1, typename T2, typename T3, typename T4> void initializationCharsHandler(T1 initializationMask, T2 j, T3 i, T4 maskXor)
{
    j -= 2;
    do
    {
        initializationMask[j--] = (i % 10) + 48;
        i /= 10;
    } while (i);
    while (initializationMask[j] != ' ')
    {
        initializationMask[j--] = ' ';
    }
    initializationGeneralCharsHandlerMutex.lock();
    initializationHandlerSemaphore = initializationHandlerSemaphore xor maskXor;
    initializationGeneralCharsHandlerMutex.unlock();
    initializationHandlerSemaphoreCond.notify_all();
}

template <typename T1, typename T2, typename T3, typename T4> void processCharsHandler(T1 processMask, T2 j, T3 i, T4 maskXor)
{
    j -= 2;
    do
    {
        processMask[j--] = (i % 10) + 48;
        i /= 10;
    } while (i);
    while (processMask[j] != ' ')
    {
        processMask[j--] = ' ';
    }
    processGeneralCharsHandlerMutex.lock();
    processHandlerSemaphore = processHandlerSemaphore xor maskXor;
    processGeneralCharsHandlerMutex.unlock();
    processHandlerSemaphoreCond.notify_all();
}

template <typename T1, typename T2, typename T3, typename T4> void processInfoCharsHandler(T1 processInfoMask, T2 j, T3 i, T4 maskXor)
{
    j -= 2;
    do
    {
        processInfoMask[j--] = (i % 10) + 48;
        i /= 10;
    } while (i);
    while (processInfoMask[j] != ' ')
    {
        processInfoMask[j--] = ' ';
    }
    processGeneralCharsHandlerInfoMutex.lock();
    processHandlerInfoSemaphore = processHandlerInfoSemaphore xor maskXor;
    processGeneralCharsHandlerInfoMutex.unlock();
    processHandlerSemaphoreInfoCond.notify_all();
}

template <typename T1, typename T2, typename T3, typename T4> void processInfoFloatCharsHandler(T1 processInfoMask, T2 j, T3 i, T4 maskXor)
{
    ostrstream temp(&processInfoMask[j] - 4, 4);
    temp << setw(4) << i;
    processGeneralCharsHandlerInfoMutex.lock();
    processHandlerInfoSemaphore = processHandlerInfoSemaphore xor maskXor;
    processGeneralCharsHandlerInfoMutex.unlock();
    processHandlerSemaphoreInfoCond.notify_all();
}

//#define NO_INFO_1
//#define NO_INFO_2

//////////// CONSTS ////////////

// TWCTE - TileWorldClimatTypeEnum
unsigned __int8 TWCTE_ARCTIC[4]{ 0,1,2,1 };
unsigned __int8 TWCTE_MODERATE[4]{ 1,2,3,2 };
unsigned __int8 TWCTE_EQUATOR[4]{ 3,3,3,3 };
unsigned __int8 EQUATOR_SOIL_RND = 2, EQUATOR_SOIL_OFFSET = 1, MODERATE_SOIL_RND = 4, MODERATE_SOIL_OFFSET = 0, ARCTIC_SOIL_RND = 2, ARCTIC_SOIL_OFFSET = 0;

unsigned __int8 SEASON_LENGHT = 100;
unsigned __int8 SEASON = 0;
unsigned __int32 TIME = 1500;

unsigned __int16 LENGHT = 10000;

unsigned __int8 GROW_SPEED = 1;
unsigned __int8 GRASS_START = 15;
unsigned __int16 GRASS_MAX = 200;
unsigned __int8 GRASS_RANGE_EXPANSION = 12;
unsigned __int8 GRASS_EXPANSION_LIMIT = 4;

unsigned __int32 COWS_COUNT = LENGHT / 4;
unsigned __int32 COWS_AGE_DIE = 0;
unsigned __int16 CHANCE_NUMERATOR = 5;
unsigned __int16 CHANCE_DENOMINATOR = 100;
unsigned __int8 BREED_TYPE = 0;

unsigned __int8 ATP_START = 8;
unsigned __int8 AGE_START = 0;
unsigned __int8 RAW_EAT_START = 0;
float AGE_MIN = 0.0001; // 0,0001
float AGE_MULT = 0.000045; // 0,00045
float AGE_T_WEIGHT = 0.0005; // 0,0005
unsigned __int8 BIRTH_COUNT = 2;
const unsigned __int8 EAT_POOL = 6;
const unsigned __int8 RAW_EAT_POOL = 8;
const unsigned __int8 ATP_POOL = 20;
const unsigned __int8 CATABOLISM = 4;
const unsigned __int8 METABOLISM_EFFICIENCY = 1;
const unsigned __int8 HOLD_PAY = 1;
const unsigned __int8 MOVE_PAY = 1;
const unsigned __int8 ENERGY_FOR_DUPLICATE = 10;

//////////// PROTOTYPES ////////////

////////////// ENUMS //////////////

///////////// STRUCTS /////////////

struct Cow
{
    unsigned __int16 Position;
    unsigned __int16 Age;

    __int16 ATP;
    __int8 RawEat;
    unsigned __int8 Size;

    unsigned __int16 ATPPool;
    unsigned __int8 RawEatPool;
    unsigned __int8 EatPool;

    unsigned __int8 MetabolismEfficiency;
    unsigned __int8 Catabolism;
    unsigned __int8 Velocity;
    unsigned __int8 Vision;

    unsigned __int8 LiveTime;
    unsigned __int8 BreedType;
    unsigned __int16 MovePos;

    unsigned __int8 Estrus;

    Cow* Next;

    Cow* Prev;
};

struct TileCow
{
    Cow* Now;

    TileCow* Next;
};

struct TileWorld
{
    unsigned __int16 Grass;
    unsigned __int8 GrassExpansionLimit;
    unsigned __int8 TileType;

    unsigned __int8 TileClimat;

    TileCow* Head;
};

struct Info
{
    unsigned __int32 WorldInfo[3]{};

    float CowInfo[5]{};
};

//////////// CONSTS ////////////

////////// FUNCTIONS //////////

void processCowSemaphoreInc()
{
    processCowSemaphoreMutex.lock();
    processCowPrintSemaphoreStart++;
    processHandlerCowCond.notify_all();
    processCowSemaphoreMutex.unlock();
}

void initializationCowSemaphoreInc()
{
    initializationCowSemaphoreMutex.lock();
    initializationCowPrintSemaphoreStart++;
    initializationHandlerCowCond.notify_all();
    initializationCowSemaphoreMutex.unlock();
}

void cowHandlerBoolXor(unsigned __int8 m)
{
    cowHandlerBoolMutex.lock();
    cowHandlerBool = cowHandlerBool xor m;
    processPrintCowCond.notify_all();
    cowHandlerBoolMutex.unlock();
}

void cowMover(TileWorld* world, Cow** Head, Cow** Tail)
{
    Cow* tempCow = *Head;

}

void cowHandler(TileWorld* world, Cow** Head, Cow** Tail)
{
    if ((*Head) != nullptr)
    {
        Cow* tempCow = *Head, * birthCow = nullptr;
        TileWorld* visionTile = nullptr, * choiceTile = nullptr;
        __int8 choice, vision;
        unsigned __int8 stillborn = 0, birthCounter = 0, tempProtein = 0, tempATP = 0;
        unsigned __int8 cowCountOnVisionTile = 0, cowCountOnChoiceTile = 0; TileCow* tempTileCow = nullptr, * tempPrevTileCow;
        __int8 move = 0;
        while (tempCow != nullptr)
        {
            if (tempCow == birthCow)
            {
                break;
            }
            choice = 0;
            vision = -1;
            if ((AGE_MIN + AGE_MULT * (1 + (AGE_T_WEIGHT * ++(tempCow->Age)))) > (float(rand()) / RAND_MAX)* tempCow->LiveTime)
            {
                COWS_AGE_DIE++;
                COWS_COUNT--;
                tempTileCow = world[tempCow->Position].Head;
                tempPrevTileCow = nullptr;
                while (tempTileCow->Now != tempCow)
                {
                    tempPrevTileCow = tempTileCow;
                    tempTileCow = tempTileCow->Next;
                }
                if (tempTileCow->Next != nullptr)
                {
                    if (tempPrevTileCow != nullptr)
                    {
                        tempPrevTileCow->Next = tempTileCow->Next;
                    }
                    else
                    {
                        world[tempCow->Position].Head = tempTileCow->Next;
                    }
                }
                else
                {
                    if (tempPrevTileCow != nullptr)
                    {
                        tempPrevTileCow->Next = nullptr;
                    }
                    else
                    {
                        world[tempCow->Position].Head = nullptr;
                    }
                }
                free(tempTileCow);
                if (tempCow->Prev == nullptr)
                {
                    if (tempCow->Next == nullptr)
                    {
                        free(tempCow);
                        *Head = nullptr;
                        *Tail = nullptr;
                        tempCow = nullptr;
                        goto ___continue_1;
                    }
                    tempCow = tempCow->Next;
                    *Head = tempCow;
                    free(tempCow->Prev);
                    tempCow->Prev = nullptr;
                }
                else if (tempCow->Next != nullptr)
                {
                    Cow* tempCow2 = tempCow;
                    tempCow = tempCow->Next;
                    tempCow->Prev = tempCow2->Prev;
                    tempCow->Prev->Next = tempCow;
                    free(tempCow2);
                }
                else
                {
                    *Tail = tempCow->Prev;
                    free(tempCow);
                    tempCow = *Tail;
                    tempCow->Next = nullptr;
                }
                goto ___continue_1;
            }
            tempTileCow = world[tempCow->Position].Head;
            while (tempTileCow != nullptr)
            {
                cowCountOnChoiceTile++;
                tempTileCow = tempTileCow->Next;
            }
            if (!BREED_TYPE && tempCow->ATP > ((tempCow->ATPPool * 3) / 4) && ((float)(tempCow->Age / tempCow->LiveTime) / 1000 > 0.2) && ((float)(tempCow->Age / tempCow->LiveTime) / 1000 < 0.4))
            {
                tempCow->Estrus = 1;
            }
            else
            {
                tempCow->Estrus = 0;
            }
            if (tempCow->Estrus)
            {
                TileCow* head;
                choiceTile = &world[(tempCow->Position + choice) < 0 ? (tempCow->Position + choice) + LENGHT : (tempCow->Position + choice) % LENGHT];
                head = choiceTile->Head;
                while (head != nullptr)
                {
                    if (head->Now == tempCow)
                    {
                        head = head->Next;
                    }
                    else if (head->Now->Estrus)
                    {
                        choice = (vision > 0 ? vision : -vision) - (choice > 0 ? choice : -choice) < 0 ? vision : choice;
                        goto ___estrus_1;
                    }
                    else
                    {
                        head = head->Next;
                    }
                }
                do
                {
                    visionTile = &world[(tempCow->Position + vision) < 0 ? (tempCow->Position + vision) + LENGHT : (tempCow->Position + vision) % LENGHT];
                    head = visionTile->Head;
                    while (head != nullptr)
                    {
                        if (head->Now->Estrus)
                        {
                            choice = (vision > 0 ? vision : -vision) - (choice > 0 ? choice : -choice) < 0 ? vision : choice;
                            goto ___estrus_1;
                        }
                        else
                        {
                            head = head->Next;
                        }
                    }
                    vision = -vision - (vision & 0x80 ? 0 : 1);
                } while (vision > -tempCow->Vision);
            }
            else if ((world[tempCow->Position].Grass < tempCow->EatPool && tempCow->ATP < ((tempCow->ATPPool * 3) / 4))  || ((cowCountOnChoiceTile == 0)))
            {
                choiceTile = &world[(tempCow->Position + choice) < 0 ? (tempCow->Position + choice) + LENGHT : (tempCow->Position + choice) % LENGHT];
                do
                {
                    visionTile = &world[(tempCow->Position + vision) < 0 ? (tempCow->Position + vision) + LENGHT : (tempCow->Position + vision) % LENGHT];
                    if (tempCow->ATP >= ((tempCow->ATPPool * 3) / 4) && (cowCountOnChoiceTile == 0))
                    {
                        cowCountOnVisionTile = 0;
                        tempTileCow = visionTile->Head;
                        while (tempTileCow != nullptr)
                        {
                            cowCountOnVisionTile++;
                            tempTileCow = tempTileCow->Next;
                        }
                        if (cowCountOnChoiceTile == 0)
                        {
                            choice = vision;
                            choiceTile = visionTile;
                            cowCountOnChoiceTile = cowCountOnVisionTile;
                        }
                    }
                    else if (choiceTile->Grass >= tempCow->EatPool && visionTile->Grass >= tempCow->EatPool)
                    {
                        choice = (vision > 0 ? vision : -vision) - (choice > 0 ? choice : -choice) < 0 ? vision : choice;
                        choiceTile = &world[(tempCow->Position + choice) < 0 ? (tempCow->Position + choice) + LENGHT : (tempCow->Position + choice) % LENGHT];
                        cowCountOnChoiceTile = 0;
                        tempTileCow = choiceTile->Head;
                        while (tempTileCow != nullptr)
                        {
                            cowCountOnChoiceTile++;
                            tempTileCow = tempTileCow->Next;
                        }
                    }
                    else if (visionTile->Grass >= tempCow->EatPool)
                    {
                        choice = vision;
                        choiceTile = &world[(tempCow->Position + choice) < 0 ? (tempCow->Position + choice) + LENGHT : (tempCow->Position + choice) % LENGHT];
                        cowCountOnChoiceTile = 0;
                        tempTileCow = choiceTile->Head;
                        while (tempTileCow != nullptr)
                        {
                            cowCountOnChoiceTile++;
                            tempTileCow = tempTileCow->Next;
                        }
                    }
                    vision = -vision - (vision & 0x80 ? 0 : 1);
                } while (vision > -tempCow->Vision);
            }
            else
            {
                choice = 0;
            }
        ___estrus_1:;
            if (choice == 0)
            {
                unsigned __int8 cost = tempCow->Size * (world[tempCow->Position].TileClimat > 0 ? (world[tempCow->Position].TileClimat > 2 ? 2 : 1) : 2);
                if (tempCow->ATP / cost)
                {
                    tempCow->ATP -= cost;
                }
                else
                {
                    COWS_COUNT--;
                    tempTileCow = world[tempCow->Position].Head;
                    tempPrevTileCow = nullptr;
                    while (tempTileCow->Now != tempCow)
                    {
                        tempPrevTileCow = tempTileCow;
                        tempTileCow = tempTileCow->Next;
                    }
                    if (tempTileCow->Next != nullptr)
                    {
                        if (tempPrevTileCow != nullptr)
                        {
                            tempPrevTileCow->Next = tempTileCow->Next;
                        }
                        else
                        {
                            world[tempCow->Position].Head = tempTileCow->Next;
                        }
                    }
                    else
                    {
                        if (tempPrevTileCow != nullptr)
                        {
                            tempPrevTileCow->Next = nullptr;
                        }
                        else
                        {
                            world[tempCow->Position].Head = nullptr;
                        }
                    }
                    free(tempTileCow);
                    if (tempCow->Prev == nullptr)
                    {
                        if (tempCow->Next == nullptr)
                        {
                            free(tempCow);
                            *Head = nullptr;
                            *Tail = nullptr;
                            tempCow = nullptr;
                            goto ___continue_1;
                        }
                        tempCow = tempCow->Next;
                        *Head = tempCow;
                        free(tempCow->Prev);
                        tempCow->Prev = nullptr;
                    }
                    else if (tempCow->Next != nullptr)
                    {
                        Cow* tempCow2 = tempCow;
                        tempCow = tempCow->Next;
                        tempCow->Prev = tempCow2->Prev;
                        tempCow->Prev->Next = tempCow;
                        free(tempCow2);
                    }
                    else
                    {
                        *Tail = tempCow->Prev;
                        free(tempCow);
                        tempCow = *Tail;
                        tempCow->Next = nullptr;
                    }
                    goto ___continue_1;
                }
            }
            else if (choice > 0)
            {
                move = (choice > tempCow->Velocity ? tempCow->Velocity : choice);
                tempTileCow = world[tempCow->Position].Head;
                tempPrevTileCow = nullptr;
                while (tempTileCow->Now != tempCow)
                {
                    tempPrevTileCow = tempTileCow;
                    tempTileCow = tempTileCow->Next;
                }
                if (tempTileCow->Next != nullptr)
                {
                    if (tempPrevTileCow != nullptr)
                    {
                        tempPrevTileCow->Next = tempTileCow->Next;
                    }
                    else
                    {
                        world[tempCow->Position].Head = tempTileCow->Next;
                    }
                    free(tempTileCow);
                }
                else
                {
                    if (tempPrevTileCow != nullptr)
                    {
                        tempPrevTileCow->Next = nullptr;
                    }
                    else
                    {
                        world[tempCow->Position].Head = nullptr;
                    }
                    free(tempTileCow);
                }
                tempCow->Position = (tempCow->Position + move) % LENGHT;
                goto ___choice;
            }
            else
            {
                move = (-choice > tempCow->Velocity ? tempCow->Velocity : choice);
                tempPrevTileCow = nullptr;
                tempTileCow = world[tempCow->Position].Head;
                while (tempTileCow->Now != tempCow)
                {
                    tempPrevTileCow = tempTileCow;
                    tempTileCow = tempTileCow->Next;
                }
                if (tempTileCow->Next != nullptr)
                {
                    if (tempPrevTileCow != nullptr)
                    {
                        tempPrevTileCow->Next = tempTileCow->Next;
                    }
                    else
                    {
                        world[tempCow->Position].Head = tempTileCow->Next;
                    }
                }
                else
                {
                    if (tempPrevTileCow != nullptr)
                    {
                        tempPrevTileCow->Next = nullptr;
                    }
                    else
                    {
                        world[tempCow->Position].Head = nullptr;
                    }
                }
                free(tempTileCow);
                tempCow->Position = ((tempCow->Position + move) < 0 ? (tempCow->Position + move) + LENGHT : (tempCow->Position + move) % LENGHT);
            ___choice:;
                tempTileCow = world[tempCow->Position].Head;
                tempPrevTileCow = nullptr;
                while (tempTileCow != nullptr)
                {
                    if (tempTileCow->Now->Position >= LENGHT)
                    {
                        tempTileCow = nullptr;
                    }
                    tempPrevTileCow = tempTileCow;
                    tempTileCow = tempTileCow->Next;

                }
                TileCow* tempLinkTileCow = (TileCow*)malloc(sizeof(TileCow));
                tempLinkTileCow->Now = tempCow;
                tempLinkTileCow->Next = nullptr;
                if (tempPrevTileCow == nullptr)
                {
                    world[tempCow->Position].Head = tempLinkTileCow;
                }
                else
                {
                    tempPrevTileCow->Next = tempLinkTileCow;
                }
                unsigned __int8 cost = (tempCow->Size + (move < 0 ? -move : move)) * (world[tempCow->Position].TileClimat > 0 ? (world[tempCow->Position].TileClimat > 2 ? 2 : 1) : 2);
                if (tempCow->ATP / cost)
                {
                    tempCow->ATP -= cost;
                }
                else
                {
                    COWS_COUNT--;
                    tempTileCow = world[tempCow->Position].Head;
                    tempPrevTileCow = nullptr;
                    while (tempTileCow->Now != tempCow)
                    {
                        tempPrevTileCow = tempTileCow;
                        tempTileCow = tempTileCow->Next;
                    }
                    if (tempTileCow->Next != nullptr)
                    {
                        if (tempPrevTileCow != nullptr)
                        {
                            tempPrevTileCow->Next = tempTileCow->Next;
                        }
                        else
                        {
                            world[tempCow->Position].Head = tempTileCow->Next;
                        }
                    }
                    else
                    {
                        if (tempPrevTileCow != nullptr)
                        {
                            tempPrevTileCow->Next = nullptr;
                        }
                        else
                        {
                            world[tempCow->Position].Head = nullptr;
                        }
                    }
                    free(tempTileCow);
                    if (tempCow->Prev == nullptr)
                    {
                        if (tempCow->Next == nullptr)
                        {
                            free(tempCow);
                            *Head = nullptr;
                            *Tail = nullptr;
                            tempCow = nullptr;
                            goto ___continue_1;
                        }
                        tempCow = tempCow->Next;
                        *Head = tempCow;
                        free(tempCow->Prev);
                        tempCow->Prev = nullptr;
                    }
                    else if (tempCow->Next != nullptr)
                    {
                        Cow* tempCow2 = tempCow;
                        tempCow = tempCow->Next;
                        tempCow->Prev = tempCow2->Prev;
                        tempCow->Prev->Next = tempCow;
                        free(tempCow2);
                    }
                    else
                    {
                        *Tail = tempCow->Prev;
                        free(tempCow);
                        tempCow = *Tail;
                        tempCow->Next = nullptr;
                    }
                    goto ___continue_1;
                }
            }
            if (tempCow->RawEatPool - tempCow->RawEat)
            {
                if (tempCow->EatPool < (tempCow->RawEatPool - tempCow->RawEat))
                {
                    if (world[tempCow->Position].Grass / tempCow->EatPool)
                    {
                        world[tempCow->Position].Grass -= tempCow->EatPool;
                        tempCow->RawEat += tempCow->EatPool;
                    }
                    else
                    {
                        tempCow->RawEat += world[tempCow->Position].Grass % tempCow->EatPool;
                        world[tempCow->Position].Grass = 0;
                        world[tempCow->Position].GrassExpansionLimit = 0;
                    }
                }
                else
                {
                    if (world[tempCow->Position].Grass / (tempCow->RawEatPool - tempCow->RawEat))
                    {
                        world[tempCow->Position].Grass -= (tempCow->RawEatPool - tempCow->RawEat);
                        tempCow->RawEat = tempCow->RawEatPool;
                    }
                    else
                    {
                        tempCow->RawEat += world[tempCow->Position].Grass % (tempCow->RawEatPool - tempCow->RawEat);
                        world[tempCow->Position].Grass = 0;
                        world[tempCow->Position].GrassExpansionLimit = 0;
                    }
                }
            }
            if (tempCow->ATPPool > tempCow->ATP)
            {
                if (tempCow->RawEat / tempCow->Catabolism)
                {
                    tempATP = tempCow->Catabolism;
                }
                else
                {
                    tempATP = tempCow->RawEat % tempCow->Catabolism;
                }
                if ((tempCow->ATPPool - tempCow->ATP) < tempATP * tempCow->MetabolismEfficiency)
                {

                    tempCow->RawEat -= (tempCow->ATPPool - tempCow->ATP) / tempCow->MetabolismEfficiency;
                    tempCow->ATP = tempCow->ATPPool;
                }
                else
                {
                    tempCow->ATP += tempATP * tempCow->MetabolismEfficiency;
                    tempCow->RawEat -= tempATP;
                }
            }

            if ((BREED_TYPE == 1))
            {
                if (tempCow->ATP >= (ENERGY_FOR_DUPLICATE * tempCow->Size) && ((float)(tempCow->Age / tempCow->LiveTime) / 1000 >= 0.2) && ((float)(tempCow->Age / tempCow->LiveTime) / 1000 <= 0.4))
                {
                    tempTileCow = world[tempCow->Position].Head;
                    tempPrevTileCow = nullptr;
                    while (tempTileCow != nullptr)
                    {
                        tempPrevTileCow = tempTileCow;
                        tempTileCow = tempTileCow->Next;
                    }
                ___duplicate_1:;
                    if (stillborn + birthCounter != BIRTH_COUNT)
                    {
                        Cow* tempCow2 = (Cow*)malloc(sizeof(Cow));
                        tempCow2->Age = AGE_START;
                        tempCow2->RawEat = RAW_EAT_START;
                        tempCow2->Position = tempCow->Position;
                        tempCow2->LiveTime = tempCow->LiveTime + ((rand() % CHANCE_DENOMINATOR < CHANCE_NUMERATOR) ? rand() % 2 ? 1 : -1 : 0);
                        if (tempCow2->LiveTime <= 0)
                        {
                            free(tempCow2);
                            tempCow2 = nullptr;
                            stillborn++;
                            goto ___duplicate_1;
                        }
                        else if (tempCow2->LiveTime > 10)
                        {
                            tempCow2->LiveTime = 10;
                        }
                        tempCow2->Size = tempCow->Size + ((rand() % CHANCE_DENOMINATOR < CHANCE_NUMERATOR) ? rand() % 2 ? 1 : -1 : 0);
                        if (tempCow2->Size <= 0)
                        {
                            free(tempCow2);
                            tempCow2 = nullptr;
                            stillborn++;
                            goto ___duplicate_1;
                        }
                        else if (tempCow2->Size > 10)
                        {
                            tempCow2->Size = 10;
                        }
                        tempCow2->ATP = ATP_START * tempCow2->Size;
                        tempCow2->ATPPool = tempCow2->Size * ATP_POOL;
                        tempCow2->RawEatPool = tempCow2->Size * RAW_EAT_POOL;
                        tempCow2->EatPool = tempCow2->Size * EAT_POOL;
                        tempCow2->Catabolism = tempCow2->Size * CATABOLISM;
                        tempCow2->MetabolismEfficiency = tempCow->MetabolismEfficiency + ((rand() % CHANCE_DENOMINATOR < CHANCE_NUMERATOR) ? rand() % 2 ? 1 : -1 : 0);
                        if (tempCow2->MetabolismEfficiency < 1)
                        {
                            free(tempCow2);
                            tempCow2 = nullptr;
                            stillborn++;
                            goto ___duplicate_1;
                        }
                        else if (tempCow2->MetabolismEfficiency > 10)
                        {
                            tempCow2->MetabolismEfficiency = 10;
                        }
                        tempCow2->Velocity = tempCow->Velocity + ((rand() % CHANCE_DENOMINATOR < CHANCE_NUMERATOR) ? rand() % 2 ? 1 : -1 : 0);
                        if (tempCow2->Velocity < 1)
                        {
                            free(tempCow2);
                            tempCow2 = nullptr;
                            stillborn++;
                            goto ___duplicate_1;
                        }
                        else if (tempCow2->Velocity > 10)
                        {
                            tempCow2->Velocity = 10;
                        }
                        tempCow2->Vision = tempCow->Vision + ((rand() % CHANCE_DENOMINATOR < CHANCE_NUMERATOR) ? rand() % 2 ? 1 : -1 : 0);
                        if (tempCow2->Vision < 1)
                        {
                            free(tempCow2);
                            tempCow2 = nullptr;
                            stillborn++;
                            goto ___duplicate_1;
                        }
                        else if (tempCow2->Vision > 10)
                        {
                            tempCow2->Vision = 10;
                        }
                        birthCounter++;
                        (*Tail)->Next = tempCow2;
                        tempCow2->Prev = (*Tail);
                        tempCow2->Next = nullptr;
                        *Tail = tempCow2;
                        TileCow* tempLinkTileCow = (TileCow*)malloc(sizeof(TileCow));
                        tempLinkTileCow->Now = tempCow2;
                        tempLinkTileCow->Next = nullptr;
                        if (tempPrevTileCow == nullptr)
                        {
                            world[tempCow2->Position].Head = tempLinkTileCow;
                        }
                        else
                        {
                            tempPrevTileCow->Next = tempLinkTileCow;
                        }
                        tempPrevTileCow = tempLinkTileCow;
                        goto ___duplicate_1;
                    }
                    else
                    {
                        COWS_COUNT += birthCounter - 1;
                        tempTileCow = world[tempCow->Position].Head;
                        tempPrevTileCow = nullptr;
                        while (tempTileCow->Now != tempCow)
                        {
                            tempPrevTileCow = tempTileCow;
                            tempTileCow = tempTileCow->Next;
                        }
                        if (tempTileCow->Next != nullptr)
                        {
                            if (tempPrevTileCow != nullptr)
                            {
                                tempPrevTileCow->Next = tempTileCow->Next;
                            }
                            else
                            {
                                world[tempCow->Position].Head = tempTileCow->Next;
                            }
                        }
                        else
                        {
                            if (tempPrevTileCow != nullptr)
                            {
                                tempPrevTileCow->Next = nullptr;
                            }
                            else
                            {
                                world[tempCow->Position].Head = nullptr;
                            }
                        }
                        free(tempTileCow);
                        birthCounter = 0;
                        stillborn = 0;
                        if (tempCow->Prev == nullptr)
                        {
                            if (tempCow->Next == nullptr)
                            {
                                free(tempCow);
                                *Head = nullptr;
                                *Tail = nullptr;
                                tempCow = nullptr;
                                goto ___continue_1;
                            }
                            tempCow = tempCow->Next;
                            *Head = tempCow;
                            free(tempCow->Prev);
                            tempCow->Prev = nullptr;
                        }
                        else if (tempCow->Next != nullptr)
                        {
                            Cow* tempCow2 = tempCow;
                            tempCow = tempCow->Next;
                            tempCow->Prev = tempCow2->Prev;
                            tempCow->Prev->Next = tempCow;
                            free(tempCow2);
                        }
                        else
                        {
                            *Tail = tempCow->Prev;
                            free(tempCow);
                            tempCow = *Tail;
                            tempCow->Next = nullptr;
                        }
                        goto ___continue_1;
                    }
                }
            }
            else if (BREED_TYPE == 0)
            {
                if ((tempCow->ATP >= (ENERGY_FOR_DUPLICATE * tempCow->Size)) && ((float)(tempCow->Age / tempCow->LiveTime) / 1000 >= 0.2) && ((float)(tempCow->Age / tempCow->LiveTime) / 1000 <= 0.4))
                {
                    Cow* tempCow3;
                    tempTileCow = world[tempCow->Position].Head;
                    while (tempTileCow != nullptr)
                    {
                        if (tempTileCow->Now == tempCow)
                        {
                            tempTileCow = tempTileCow->Next;
                            continue;
                        }
                        else
                        {
                            TileCow* tempTileCow2 = world[tempCow->Position].Head;
                            tempPrevTileCow = nullptr;
                            while (tempTileCow2 != nullptr)
                            {
                                tempPrevTileCow = tempTileCow2;
                                tempTileCow2 = tempTileCow2->Next;
                            }
                            tempCow3 = tempTileCow->Now;
                            tempTileCow = tempTileCow->Next;
                            if ((tempCow3->ATP >= (ENERGY_FOR_DUPLICATE * tempCow3->Size)) && ((float)(tempCow->Age / tempCow->LiveTime) / 1000 >= 0.2) && ((float)(tempCow->Age / tempCow->LiveTime) / 1000 <= 0.4))
                            {
                                tempCow->ATP -= ((ENERGY_FOR_DUPLICATE * tempCow->Size) >> 1);
                                tempCow3->ATP -= ((ENERGY_FOR_DUPLICATE * tempCow3->Size) >> 1);
                            ___duplicate_2:;
                                if (stillborn + birthCounter != BIRTH_COUNT - 1)
                                {
                                    Cow* tempCow2 = (Cow*)malloc(sizeof(Cow));
                                    tempCow2->Age = AGE_START;
                                    tempCow2->RawEat = RAW_EAT_START;
                                    tempCow2->Position = tempCow->Position;
                                    tempCow2->LiveTime = (tempCow->LiveTime + tempCow3->LiveTime) / 2 + (tempCow->LiveTime + tempCow3->LiveTime) % 2 + ((rand() % CHANCE_DENOMINATOR < CHANCE_NUMERATOR) ? rand() % 2 ? 1 : -1 : 0);
                                    if (tempCow2->LiveTime <= 0)
                                    {
                                        free(tempCow2);
                                        tempCow2 = nullptr;
                                        stillborn++;
                                        goto ___duplicate_2;
                                    }
                                    else if (tempCow2->LiveTime > 10)
                                    {
                                        tempCow2->LiveTime = 10;
                                    }
                                    tempCow2->Size = (tempCow->Size + tempCow3->Size) / 2 + (tempCow->Size + tempCow3->Size) % 2 + ((rand() % CHANCE_DENOMINATOR < CHANCE_NUMERATOR) ? rand() % 2 ? 1 : -1 : 0);
                                    if (tempCow2->Size <= 0)
                                    {
                                        free(tempCow2);
                                        tempCow2 = nullptr;
                                        stillborn++;
                                        goto ___duplicate_2;
                                    }
                                    else if (tempCow2->Size > 10)
                                    {
                                        tempCow2->Size = 10;
                                    }
                                    tempCow2->ATP = ATP_START * tempCow2->Size;
                                    tempCow2->ATPPool = tempCow2->Size * ATP_POOL;
                                    tempCow2->RawEatPool = tempCow2->Size * RAW_EAT_POOL;
                                    tempCow2->EatPool = tempCow2->Size * EAT_POOL;
                                    tempCow2->Catabolism = tempCow2->Size * CATABOLISM;
                                    tempCow2->MetabolismEfficiency = (tempCow->MetabolismEfficiency + tempCow3->MetabolismEfficiency) / 2 + (tempCow->MetabolismEfficiency + tempCow3->MetabolismEfficiency) % 2 + ((rand() % CHANCE_DENOMINATOR < CHANCE_NUMERATOR) ? rand() % 2 ? 1 : -1 : 0);
                                    if (tempCow2->MetabolismEfficiency < 1)
                                    {
                                        free(tempCow2);
                                        tempCow2 = nullptr;
                                        stillborn++;
                                        goto ___duplicate_2;
                                    }
                                    else if (tempCow2->MetabolismEfficiency > 10)
                                    {
                                        tempCow2->MetabolismEfficiency = 10;
                                    }
                                    tempCow2->Velocity = (tempCow->Velocity + tempCow3->Velocity) / 2 + (tempCow->Velocity + tempCow3->Velocity) % 2 + ((rand() % CHANCE_DENOMINATOR < CHANCE_NUMERATOR) ? rand() % 2 ? 1 : -1 : 0);
                                    if (tempCow2->Velocity < 1)
                                    {
                                        free(tempCow2);
                                        tempCow2 = nullptr;
                                        stillborn++;
                                        goto ___duplicate_2;
                                    }
                                    else if (tempCow2->Velocity > 10)
                                    {
                                        tempCow2->Velocity = 10;
                                    }
                                    tempCow2->Vision = (tempCow->Vision + tempCow3->Vision) / 2 + (tempCow->Vision + tempCow3->Vision) % 2 + ((rand() % CHANCE_DENOMINATOR < CHANCE_NUMERATOR) ? rand() % 2 ? 1 : -1 : 0);
                                    if (tempCow2->Vision < 1)
                                    {
                                        free(tempCow2);
                                        tempCow2 = nullptr;
                                        stillborn++;
                                        goto ___duplicate_2;
                                    }
                                    else if (tempCow2->Vision > 10)
                                    {
                                        tempCow2->Vision = 10;
                                    }
                                    birthCounter++;
                                    (*Tail)->Next = tempCow2;
                                    tempCow2->Prev = (*Tail);
                                    tempCow2->Next = nullptr;
                                    *Tail = tempCow2;
                                    TileCow* tempLinkTileCow = (TileCow*)malloc(sizeof(TileCow));
                                    tempLinkTileCow->Now = tempCow2;
                                    tempLinkTileCow->Next = nullptr;
                                    if (tempPrevTileCow == nullptr)
                                    {
                                        world[tempCow2->Position].Head = tempLinkTileCow;
                                    }
                                    else
                                    {
                                        tempPrevTileCow->Next = tempLinkTileCow;
                                    }
                                    tempPrevTileCow = tempLinkTileCow;
                                    goto ___duplicate_2;
                                }
                                else
                                {
                                    COWS_COUNT += birthCounter;
                                    birthCounter = 0;
                                    stillborn = 0;
                                    tempCow = tempCow->Next;
                                    goto ___continue_1;
                                }
                            }
                        }
                    }
                }
            }
            tempCow = tempCow->Next;
        ___continue_1:;
        }
    }
    cowHandlerBoolXor(0b00000001);
}

void tileHandler(TileWorld* world, unsigned __int32* time)
{
    unsigned __int16 tileIterator = 0xFFFF;
    __int8 grassRangeExpansion = 0;
    while (++tileIterator < LENGHT)
    {
        if ((tileIterator >= LENGHT * 3 / 40) && (tileIterator < LENGHT * 7 / 40) || (tileIterator >= LENGHT * 13 / 40) && (tileIterator < LENGHT * 17 / 40) || (tileIterator >= LENGHT * 23 / 40) && (tileIterator < LENGHT * 27 / 40) || (tileIterator >= LENGHT * 33 / 40) && (tileIterator < LENGHT * 37 / 40))
        {
            world[tileIterator].TileClimat = TWCTE_MODERATE[((*time / SEASON_LENGHT) % 4 + ((tileIterator < (LENGHT / 2)) ? 0 : 2)) % 4];
        }
        else if ((tileIterator >= LENGHT * 7 / 40) && (tileIterator < LENGHT * 13 / 40) || (tileIterator >= LENGHT * 27 / 40) && (tileIterator < LENGHT * 33 / 40))
        {
            world[tileIterator].TileClimat = TWCTE_ARCTIC[((*time / SEASON_LENGHT) % 4 + ((tileIterator < (LENGHT / 2) ? 0 : 2))) % 4];
        }
        else
        {
            world[tileIterator].TileClimat = TWCTE_EQUATOR[((*time / SEASON_LENGHT) % 4 + ((tileIterator < (LENGHT / 2) ? 0 : 2))) % 4];
        }
        if (world[tileIterator].Grass)
        {
            world[tileIterator].Grass = ((world[tileIterator].Grass + (GROW_SPEED * world[tileIterator].TileClimat * world[tileIterator].TileType)) < (GRASS_MAX * world[tileIterator].TileType)) ? world[tileIterator].Grass + GROW_SPEED * (world[tileIterator].TileClimat + world[tileIterator].TileType) : GRASS_MAX * world[tileIterator].TileType;
            continue;
        }
        if (!(world[tileIterator].TileType))
        {
            continue;
        }
        do
        {
            if (world[((tileIterator + grassRangeExpansion) < 0) ? (tileIterator + grassRangeExpansion) + LENGHT : (tileIterator + grassRangeExpansion) % LENGHT].GrassExpansionLimit)
            {
                world[tileIterator].Grass++;
                world[((tileIterator + grassRangeExpansion) < 0) ? (tileIterator + grassRangeExpansion) + LENGHT : (tileIterator + grassRangeExpansion) % LENGHT].GrassExpansionLimit--;
                grassRangeExpansion = 0;
                break;
            }
            grassRangeExpansion = -grassRangeExpansion - (grassRangeExpansion & 0x80 ? 0 : 1);
        } while (grassRangeExpansion <= GRASS_RANGE_EXPANSION);
        grassRangeExpansion = 0;
    }
    tileIterator = 0xFFFF;
    while (++tileIterator < LENGHT)
    {
        if (world[tileIterator].Grass)
        {
            world[tileIterator].GrassExpansionLimit = GRASS_EXPANSION_LIMIT;
        }
    }
    cowHandlerBoolXor(0b00000010);
}

void processCSVHandler(unsigned __int8 processCSVCounter, char* processCSVName, char* processCSVType)
{
    unsigned __int8 processCSVNameIterator = 8, i = 0;
    char processCSVNamePiece[3]{};
    while (processCSVCounter)
    {
        processCSVNamePiece[i] = (char)((processCSVCounter % 10) + 48);
        processCSVCounter /= 10;
        if (processCSVCounter)
        {
            i++;
        }
    }
    do
    {
        processCSVName[processCSVNameIterator] = processCSVNamePiece[i];
        if (!i)
        {
            processCSVNameIterator++;
            break;
        }
        processCSVNameIterator++;
        i--;
    } while (true);
    while (i != 5)
    {
        processCSVName[processCSVNameIterator++] = processCSVType[i++];
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void processFilePrinter(FILE* file, char* str)
{
    unsigned __int8 j = 0xFF;
    processPrintMutex.lock();
    while (str[++j] != '\000')
    {
        if (str[j] == '|') { fprintf_s(file, "%c", ' '); j++; }
        fprintf_s(file, "%c", str[j]);
    }
    processPrintMutex.unlock();
}

void processFilePrinterMaskWorld(FILE* file, char* str)
{
    unsigned __int8 j = 0;
    unsigned __int16 i = 0, counter, counterMax = 400;
    unsigned __int32 j2 = 0;
    counter = LENGHT / counterMax;
    while (counter)
    {
        while (str[j] != '\000')
        {
            j++;
        }
        char* str2 = (char*)malloc(sizeof(char) * (counterMax * j + 1));
        str2[counterMax * j] = '\000';
        j = 0;
        j2 = 0;
    ___cycle_1:;
        unique_lock<mutex> lk(processPrintMutex);
        processPrintCond.wait(lk, []()->bool {return processPrintBool; });
        while (str[j] != '\000')
        {
            if (str[j] == '|')
            {
                str2[j2] = ' ';
                j++;
                j2++;
                continue;
            }
            str2[j2++] = str[j++];
        }
        processPrintBool = false;
        processPrintCond.notify_all();
        j = 0;
        lk.unlock();
        if (++i < counterMax)
        {
            goto ___cycle_1;
        }
        i = 0;
        j2 = 0;
        fprintf_s(file, "%s", str2);
        free(str2);
        counter--;
    }
    counterMax = LENGHT % counterMax;
    if (counterMax)
    {
        while (str[j] != '\000')
        {
            j++;
        }
        char* str2 = (char*)malloc(sizeof(char) * (counterMax * j + 1));
        str2[counterMax * j] = '\000';
        j = 0;
        j2 = 0;
    ___cycle_2:;
        unique_lock<mutex> lk2(processPrintMutex);
        processPrintCond.wait(lk2, []()->bool {return processPrintBool; });
        while (str[j] != '\000')
        {
            if (str[j] == '|')
            {
                str2[j2] = ' ';
                j++;
                j2++;
                continue;
            }
            str2[j2++] = str[j++];
        }
        processPrintBool = false;
        processPrintCond.notify_all();
        j = 0;
        lk2.unlock();
        if (++i < counterMax)
        {
            goto ___cycle_2;
        }
        j2 = 0;
        fprintf_s(file, "%s", str2);
        free(str2);
    }
}

void processFilePrinterMaskCow(FILE* file, char* str, Cow* tempCow)
{
    unsigned __int8 j = 0, i, counterMax = 100, counter = 0;
    unsigned __int32  j2 = 0;
    i = COWS_COUNT / counterMax;
    while (i)
    {
        while (str[j] != '\000')
        {
            j++;
        }
        char* str2 = (char*)malloc(sizeof(char) * (counterMax * j + 1));
        str2[counterMax * j] = '\000';
        j = 0;
        j2 = 0;
    ___cycle_1:;
        unique_lock<mutex> lk(processPrintMutex);
        processPrintCond.wait(lk, []()->bool {return processPrintBool; });
        while (str[j] != '\000')
        {
            if (str[j] == '|')
            {
                str2[j2] = ' ';
                j++;
                j2++;
                continue;
            }
            str2[j2++] = str[j++];
        }
        processPrintBool = false;
        processPrintCond.notify_all();
        j = 0;
        lk.unlock();
        if (++counter != counterMax)
        {
            tempCow = tempCow->Next;
            goto ___cycle_1;
        }
        counter = 0;
        fprintf_s(file, "%s", str2);
        free(str2);
        i--;
    }
    counter = COWS_COUNT % counterMax;
    if (counter)
    {
        while (str[j] != '\000')
        {
            j++;
        }
        char* str2 = (char*)malloc(sizeof(char) * (counter * j + 1));
        str2[counter * j] = '\000';
        j2 = 0;
        j = 0;
    ___cycle_2:;
        unique_lock<mutex> lk(processPrintMutex);
        processPrintCond.wait(lk, []()->bool {return processPrintBool; });
        while (str[j] != '\000')
        {
            if (str[j] == '|')
            {
                str2[j2] = ' ';
                j++;
                j2++;
                continue;
            }
            str2[j2++] = str[j++];
        }
        processPrintBool = false;
        processPrintCond.notify_all();
        j = 0;
        lk.unlock();
        if (--counter)
        {
            tempCow = tempCow->Next;
            goto ___cycle_2;
        }
        fprintf_s(file, "%s", str2);
        free(str2);
    }
}

void processOutputWorldHandler(char* processWorldMask, TileWorld* world, FILE* processAllFile)
{
    unsigned __int16 i = 0, j = 0;
    unsigned __int8 count = 0;
___cycle:;
    unique_lock<mutex> lk(processOutputWorldHandlerMutex);
    thread gCHIStore, gCHGrassStore, gCHGrassExpansionLimitStore, gCHTileTypeStore, gCHTileClimatStore;
    while (processWorldMask[j++] != '\n')
    {
        if (processWorldMask[j] == '|')
        {
            if (!count)
            {
                processPrintCond.wait(lk, []()->bool {return !processPrintBool; });
                thread gCHI([processWorldMask, j, i]()->void {processCharsHandler(processWorldMask, j, i, (unsigned __int8)0b00000001); });
                gCHIStore = move(gCHI);
                count++;
            }
            else if (count == 1)
            {
                thread gCHGrass([processWorldMask, j, i, world]()->void {processCharsHandler(processWorldMask, j, world[i].Grass, (unsigned __int8)0b00000010); });
                gCHGrassStore = move(gCHGrass);
                count++;
            }
            else if (count == 2)
            {
                thread gCHGrassExpansionLimit([processWorldMask, j, i]()->void {processCharsHandler(processWorldMask, j, GRASS_EXPANSION_LIMIT, (unsigned __int8)0b00000100); });
                gCHGrassExpansionLimitStore = move(gCHGrassExpansionLimit);
                count++;
            }
            else if (count == 3)
            {
                thread gCHTileType([processWorldMask, j, i, world]()->void {processCharsHandler(processWorldMask, j, world[i].TileType, (unsigned __int8)0b00001000); });
                gCHTileTypeStore = move(gCHTileType);
                count++;
            }
            else
            {
                thread gCHTileClimat([processWorldMask, j, i, world]()->void {processCharsHandler(processWorldMask, j, world[i].TileClimat, (unsigned __int8)0b00010000); });
                gCHTileClimatStore = move(gCHTileClimat);
                count = 0;
            }
        }
    }
    gCHIStore.join();
    gCHGrassStore.join();
    gCHGrassExpansionLimitStore.join();
    gCHTileTypeStore.join();
    gCHTileClimatStore.join();
    processHandlerSemaphoreCond.wait(lk, []()->bool {return !processHandlerSemaphore; });
    processHandlerSemaphore = (unsigned __int16)0x001F;
    processPrintBool = true;
    processPrintCond.notify_all();
    lk.unlock();
    j = 0;
    if (++i < LENGHT)
    {
        goto ___cycle;
    }
    processHandlerSemaphore = (unsigned __int16)0x01FF;
}

void processFilePrintWorld(void (*processFilePrinter)(FILE*, char*), void (*processOutputWorldHandler)(char*, TileWorld*, FILE*), FILE* processAllFile, char* processWorldHead, char* processWorldMask, char* processWorldTail, TileWorld* world)
{
    thread filePrintHead(processFilePrinter, processAllFile, processWorldHead);
    filePrintHead.join();
    thread filePrintMask(processOutputWorldHandler, processWorldMask, world, processAllFile);
    thread filePrintModMask(processFilePrinterMaskWorld, processAllFile, processWorldMask);
    filePrintMask.join();
    filePrintModMask.join();
    thread filePrintTail(processFilePrinter, processAllFile, processWorldTail);
    filePrintTail.join();
    thread cowSemaphoreAdd(processCowSemaphoreInc);
    cowSemaphoreAdd.join();
}

void processOutputCowHandler(char* processCowMask, Cow* tempCow, FILE* processAllFile)
{
    unsigned __int16 i = 0, j = 0;
    unsigned __int8 count = 0;
___cycle:;
    unique_lock<mutex> lk(processOutputExistCowHandlerMutex);
    thread gCHIStore, gCHAgeStore, gCHATPStore, gCHPositionStore, gCHSizeStore, gCHMetabolismEfficiencyStore, gCHCatabolismStore, gCHVelocityStore, gCHVisionStore;
    while (processCowMask[j++] != '\n')
    {
        if (processCowMask[j] == '|')
        {
            if (!count)
            {
                processPrintCond.wait(lk, []()->bool {return !processPrintBool; });
                thread gCHI([processCowMask, j, i]()->void {processCharsHandler(processCowMask, j, i, (unsigned __int16)0x0001); });
                gCHIStore = move(gCHI);
                count++;
            }
            else if (count == 1)
            {
                thread gCHAge([processCowMask, j, tempCow]()->void {processCharsHandler(processCowMask, j, (unsigned __int16)tempCow->Age, (unsigned __int16)0x0002); });
                gCHAgeStore = move(gCHAge);
                count++;
            }
            else if (count == 2)
            {
                thread gCHATP([processCowMask, j, tempCow]()->void {processCharsHandler(processCowMask, j, (__int16)tempCow->ATP, (unsigned __int16)0x0004); });
                gCHATPStore = move(gCHATP);
                count++;
            }
            else if (count == 3)
            {
                thread gCHPosition([processCowMask, j, tempCow]()->void {processCharsHandler(processCowMask, j, (unsigned __int16)tempCow->Position, (unsigned __int16)0x0008); });
                gCHPositionStore = move(gCHPosition);
                count++;
            }
            else if (count == 4)
            {
                thread gCHSize([processCowMask, j, tempCow]()->void {processCharsHandler(processCowMask, j, (unsigned __int8)tempCow->Size, (unsigned __int16)0x0010); });
                gCHSizeStore = move(gCHSize);
                count++;
            }
            else if (count == 5)
            {
                thread gCHMetabolismEfficiency([processCowMask, j, tempCow]()->void {processCharsHandler(processCowMask, j, (unsigned __int8)tempCow->MetabolismEfficiency, (unsigned __int16)0x0020); });
                gCHMetabolismEfficiencyStore = move(gCHMetabolismEfficiency);
                count++;
            }
            else if (count == 6)
            {
                thread gCHCatabolism([processCowMask, j, tempCow]()->void {processCharsHandler(processCowMask, j, (unsigned __int8)tempCow->Catabolism, (unsigned __int16)0x0040); });
                gCHCatabolismStore = move(gCHCatabolism);
                count++;
            }
            else if (count == 7)
            {
                thread gCHVelocity([processCowMask, j, tempCow]()->void {processCharsHandler(processCowMask, j, (unsigned __int8)tempCow->Velocity, (unsigned __int16)0x0080); });
                gCHVelocityStore = move(gCHVelocity);
                count++;
            }
            else
            {
                thread gCHVision([processCowMask, j, tempCow]()->void {processCharsHandler(processCowMask, j, (unsigned __int8)tempCow->Vision, (unsigned __int16)0x0100); });
                gCHVisionStore = move(gCHVision);
                count = 0;
            }
        }
    }
    gCHIStore.join();
    gCHAgeStore.join();
    gCHATPStore.join();
    gCHPositionStore.join();
    gCHSizeStore.join();
    gCHMetabolismEfficiencyStore.join();
    gCHCatabolismStore.join();
    gCHVelocityStore.join();
    gCHVisionStore.join();
    processHandlerSemaphoreCond.wait(lk, []()->bool {return !processHandlerSemaphore; });
    processPrintBool = true;
    processHandlerSemaphore = (unsigned __int16)0x01FF;
    lk.unlock();
    j = 0;
    processPrintCond.notify_all();
    if (tempCow->Next != nullptr)
    {
        i++;
        tempCow = tempCow->Next;
        goto ___cycle;
    }
    processHandlerSemaphore = (unsigned __int16)0x001F;
}

void processFilePrintCow(void (*processFilePrinter)(FILE*, char*), void (*processOutputCowHandler)(char*, Cow*, FILE*), FILE* processAllFile, char* processHead, char* processMask, char* processTail, Cow* Head)
{
    unique_lock<mutex> lk(processCowPreprocessorMutex);
    if (Head != nullptr)
    {
        processHandlerCowCond.wait(lk, []()->bool {return (processCowPrintSemaphoreStart == 1); });
        processCowPrintSemaphoreStart = 0;
        thread filePrintHead(processFilePrinter, processAllFile, processHead);
        filePrintHead.join();
        thread filePrintMask(processFilePrinterMaskCow, processAllFile, processMask, Head);
        thread filePrintModMaskCow(processOutputCowHandler, processMask, Head, processAllFile);
        filePrintModMaskCow.join();
        processHandlerCowCond.wait(lk, []()->bool {return processCowPrintSemaphoreContinue; });
        processCowPrintSemaphoreContinue = false;
        filePrintMask.join();
        thread filePrintTail(processFilePrinter, processAllFile, processTail);
        filePrintTail.join();
    }
    else
    {
        processHandlerCowCond.wait(lk, []()->bool {return (processCowPrintSemaphoreStart == 1); });
        processCowPrintSemaphoreStart = 0;
        processHandlerSemaphore = (unsigned __int16)0x001F;
    }
    lk.unlock();
    cowHandlerBoolXor(0b00000011);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void initializationFilePrinter(FILE* file, char* str)
{
    unsigned __int8 j = 0xFF;
    initializationPrintMutex.lock();
    while (str[++j] != '\000')
    {
        if (str[j] == '|') { fprintf_s(file, "%c", ' '); j++; }
        fprintf_s(file, "%c", str[j]);
    }
    initializationPrintMutex.unlock();
}

void initializationFilePrinterMaskWorld(FILE* file, char* str)
{
    unsigned __int8 j = 0;
    unsigned __int16 i = 0, counter, counterMax = 400;
    unsigned __int32 j2 = 0;
    counter = LENGHT / counterMax;
    while (counter)
    {
        while (str[j] != '\000')
        {
            j++;
        }
        char* str2 = (char*)malloc(sizeof(char) * (counterMax * j + 1));
        str2[counterMax * j] = '\000';
        j = 0;
        j2 = 0;
    ___cycle_1:;
        unique_lock<mutex> lk(initializationPrintMutex);
        initializationPrintCond.wait(lk, []()->bool {return initializationPrintBool; });
        while (str[j] != '\000')
        {
            if (str[j] == '|')
            {
                str2[j2] = ' ';
                j++;
                j2++;
                continue;
            }
            str2[j2++] = str[j++];
        }
        initializationPrintBool = false;
        initializationPrintCond.notify_all();
        j = 0;
        lk.unlock();
        if (++i < counterMax)
        {
            goto ___cycle_1;
        }
        i = 0;
        j2 = 0;
        fprintf_s(file, "%s", str2);
        free(str2);
        counter--;
    }
    counterMax = LENGHT % counterMax;
    if (counterMax)
    {
        while (str[j] != '\000')
        {
            j++;
        }
        char* str2 = (char*)malloc(sizeof(char) * (counterMax * j + 1));
        str2[counterMax * j] = '\000';
        j = 0;
        j2 = 0;
    ___cycle_2:;
        unique_lock<mutex> lk2(initializationPrintMutex);
        initializationPrintCond.wait(lk2, []()->bool {return initializationPrintBool; });
        while (str[j] != '\000')
        {
            if (str[j] == '|')
            {
                str2[j2] = ' ';
                j++;
                j2++;
                continue;
            }
            str2[j2++] = str[j++];
        }
        initializationPrintBool = false;
        initializationPrintCond.notify_all();
        j = 0;
        lk2.unlock();
        if (++i < counterMax)
        {
            goto ___cycle_2;
        }
        j2 = 0;
        fprintf_s(file, "%s", str2);
        free(str2);
    }
}

void initializationFilePrinterMaskCow(FILE* file, char* str, Cow* tempCow)
{
    unsigned __int16 j = 0, i, counterMax = 100, counter = 0;
    unsigned __int32  j2 = 0;
    i = COWS_COUNT / counterMax;
    while (i)
    {
        while (str[j] != '\000')
        {
            j++;
        }
        char* str2;
        do
        {
            str2 = (char*)malloc(sizeof(char) * (counterMax * j + 1));
        } while (str2 == nullptr);
        str2[counterMax * j] = '\000';
        j = 0;
        j2 = 0;
    ___cycle_1:;
        unique_lock<mutex> lk(initializationPrintMutex);
        initializationPrintCond.wait(lk, []()->bool {return initializationPrintBool; });
        while (str[j] != '\000')
        {
            if (str[j] == '|')
            {
                str2[j2] = ' ';
                j++;
                j2++;
                continue;
            }
            str2[j2++] = str[j++];
        }
        initializationPrintBool = false;
        initializationPrintCond.notify_all();
        j = 0;
        lk.unlock();
        if (++counter != counterMax)
        {
            tempCow = tempCow->Next;
            goto ___cycle_1;
        }
        counter = 0;
        fprintf_s(file, "%s", str2);
        free(str2);
        i--;
    }
    counter = COWS_COUNT % counterMax;
    if (counter)
    {
        while (str[j] != '\000')
        {
            j++;
        }
        char* str2 = (char*)malloc(sizeof(char) * (counter * j + 1));
        str2[counter * j] = '\000';
        j2 = 0;
        j = 0;
    ___cycle_2:;
        unique_lock<mutex> lk(initializationPrintMutex);
        initializationPrintCond.wait(lk, []()->bool {return initializationPrintBool; });
        while (str[j] != '\000')
        {
            if (str[j] == '|')
            {
                str2[j2] = ' ';
                j++;
                j2++;
                continue;
            }
            str2[j2++] = str[j++];
        }
        initializationPrintBool = false;
        initializationPrintCond.notify_all();
        j = 0;
        lk.unlock();
        if (--counter)
        {
            tempCow = tempCow->Next;
            goto ___cycle_2;
        }
        fprintf_s(file, "%s", str2);
        free(str2);
    }
}

void initializationOutputWorldHandler(char* initializationWorldMask, TileWorld* world, FILE* initializationAllFile)
{
    unsigned __int16 i = 0, j = 0;
    unsigned __int8 count = 0;
___cycle:;
    unique_lock<mutex> lk(initializationHandlerMutex);
    thread gCHIStore, gCHGrassStore, gCHGrassExpansionLimitStore, gCHTileTypeStore, gCHTileClimatStore;
    while (initializationWorldMask[j++] != '\n')
    {
        if (initializationWorldMask[j] == '|')
        {
            if (!count)
            {
                initializationPrintCond.wait(lk, []()->bool {return !initializationPrintBool; });
                thread gCHI([initializationWorldMask, j, i]()->void {initializationCharsHandler(initializationWorldMask, j, i, (unsigned __int16)0x0001); });
                gCHIStore = move(gCHI);
                count++;
            }
            else if (count == 1)
            {
                thread gCHGrass([initializationWorldMask, j, i]()->void {initializationCharsHandler(initializationWorldMask, j, GRASS_START, (unsigned __int16)0x0002); });
                gCHGrassStore = move(gCHGrass);
                count++;
            }
            else if (count == 2)
            {
                thread gCHGrassExpansionLimit([initializationWorldMask, j, i]()->void {initializationCharsHandler(initializationWorldMask, j, GRASS_EXPANSION_LIMIT, (unsigned __int16)0x0004); });
                gCHGrassExpansionLimitStore = move(gCHGrassExpansionLimit);
                count++;
            }
            else if (count == 3)
            {
                thread gCHTileType([initializationWorldMask, j, i, world]()->void {initializationCharsHandler(initializationWorldMask, j, world[i].TileType, (unsigned __int16)0x0008); });
                gCHTileTypeStore = move(gCHTileType);
                count++;
            }
            else
            {
                thread gCHTileClimat([initializationWorldMask, j, i, world]()->void {initializationCharsHandler(initializationWorldMask, j, world[i].TileClimat, (unsigned __int16)0x0010); });
                gCHTileClimatStore = move(gCHTileClimat);
                count = 0;
            }
        }
    }
    gCHIStore.join();
    gCHGrassStore.join();
    gCHGrassExpansionLimitStore.join();
    gCHTileTypeStore.join();
    gCHTileClimatStore.join();
    initializationHandlerSemaphoreCond.wait(lk, []()->bool {return !initializationHandlerSemaphore; });
    initializationPrintBool = true;
    initializationHandlerSemaphore = (unsigned __int16)0x001F;
    initializationPrintCond.notify_all();
    lk.unlock();
    j = 0;
    if (++i < LENGHT)
    {
        goto ___cycle;
    }
    initializationHandlerSemaphore = (unsigned __int16)0x01FF;
}

void initializationFilePrintWorld(FILE* initializationAllFile, char* initializationWorldHead, char* initializationWorldMask, char* initializationWorldTail, TileWorld* world)
{
    initializationFilePrinter(initializationAllFile, initializationWorldHead);
    thread filePrintMask(initializationOutputWorldHandler, initializationWorldMask, world, initializationAllFile);
    thread filePrintModMask(initializationFilePrinterMaskWorld, initializationAllFile, initializationWorldMask);
    filePrintMask.join();
    filePrintModMask.join();
    initializationFilePrinter(initializationAllFile, initializationWorldTail);
    thread cowSemaphoreAdd(initializationCowSemaphoreInc);
    cowSemaphoreAdd.join();
}

void initializationOutputCowHandler(char* initializationCowMask, Cow* tempCow, FILE* initializationAllFile)
{
    unsigned __int16 i = 0, j = 0;
    unsigned __int8 count = 0;
___cycle:;
    unique_lock<mutex> lk(initializationHandlerMutex);
    thread gCHIStore, gCHAgeStore, gCHATPStore, gCHPositionStore, gCHSizeStore, gCHMetabolismEfficiencyStore, gCHCatabolismStore, gCHVelocityStore, gCHVisionStore;
    while (initializationCowMask[j++] != '\n')
    {
        if (initializationCowMask[j] == '|')
        {
            if (!count)
            {
                initializationPrintCond.wait(lk, []()->bool {return !initializationPrintBool; });
                thread gCHI([initializationCowMask, j, i]()->void {initializationCharsHandler(initializationCowMask, j, i, (unsigned __int16)0x0001); });
                gCHIStore = move(gCHI);
                count++;
            }
            else if (count == 1)
            {
                thread gCHAge([initializationCowMask, j, tempCow]()->void {initializationCharsHandler(initializationCowMask, j, (unsigned __int16)tempCow->Age, (unsigned __int16)0x0002); });
                gCHAgeStore = move(gCHAge);
                count++;
            }
            else if (count == 2)
            {
                thread gCHATP([initializationCowMask, j, tempCow]()->void {initializationCharsHandler(initializationCowMask, j, (__int8)tempCow->ATP, (unsigned __int16)0x0004); });
                gCHATPStore = move(gCHATP);
                count++;
            }
            else if (count == 3)
            {
                thread gCHPosition([initializationCowMask, j, tempCow]()->void {initializationCharsHandler(initializationCowMask, j, (unsigned __int16)tempCow->Position, (unsigned __int16)0x0008); });
                gCHPositionStore = move(gCHPosition);
                count++;
            }
            else if (count == 4)
            {
                thread gCHSize([initializationCowMask, j, tempCow]()->void {initializationCharsHandler(initializationCowMask, j, (unsigned __int8)tempCow->Size, (unsigned __int16)0x0010); });
                gCHSizeStore = move(gCHSize);
                count++;
            }
            else if (count == 5)
            {
                thread gCHMetabolismEfficiency([initializationCowMask, j, tempCow]()->void {initializationCharsHandler(initializationCowMask, j, (unsigned __int8)tempCow->MetabolismEfficiency, (unsigned __int16)0x0020); });
                gCHMetabolismEfficiencyStore = move(gCHMetabolismEfficiency);
                count++;
            }
            else if (count == 6)
            {
                thread gCHCatabolism([initializationCowMask, j, tempCow]()->void {initializationCharsHandler(initializationCowMask, j, (unsigned __int8)tempCow->Catabolism, (unsigned __int16)0x0040); });
                gCHCatabolismStore = move(gCHCatabolism);
                count++;
            }
            else if (count == 7)
            {
                thread gCHVelocity([initializationCowMask, j, tempCow]()->void {initializationCharsHandler(initializationCowMask, j, (unsigned __int8)tempCow->Velocity, (unsigned __int16)0x0080); });
                gCHVelocityStore = move(gCHVelocity);
                count++;
            }
            else
            {
                thread gCHVision([initializationCowMask, j, tempCow]()->void {initializationCharsHandler(initializationCowMask, j, (unsigned __int8)tempCow->Vision, (unsigned __int16)0x0100); });
                gCHVisionStore = move(gCHVision);
                count = 0;
            }
        }
    }
    gCHIStore.join();
    gCHAgeStore.join();
    gCHATPStore.join();
    gCHPositionStore.join();
    gCHSizeStore.join();
    gCHMetabolismEfficiencyStore.join();
    gCHCatabolismStore.join();
    gCHVelocityStore.join();
    gCHVisionStore.join();
    initializationHandlerSemaphoreCond.wait(lk, []()->bool {return !initializationHandlerSemaphore; });
    initializationPrintBool = true;
    initializationHandlerSemaphore = (unsigned __int16)0x01FF;
    initializationPrintCond.notify_all();
    lk.unlock();
    j = 0;
    if (tempCow->Next != nullptr)
    {
        i++;
        tempCow = tempCow->Next;
        goto ___cycle;
    }
    initializationHandlerSemaphore = (unsigned __int16)0x0000;
}

void initializationFilePrintCow(FILE* initializationAllFile, char* initializationHead, char* initializationMask, char* initializationTail, Cow* Head)
{

    if (Head != nullptr)
    {
        unique_lock<mutex> lk(initializationCowPreprocessorMutex);
        initializationHandlerCowCond.wait(lk, []()->bool {return initializationCowPrintSemaphoreStart == 2; });
        initializationFilePrinter(initializationAllFile, initializationHead);
        thread filePrintMask(initializationFilePrinterMaskCow, initializationAllFile, initializationMask, Head);
        thread filePrintModMaskCow(initializationOutputCowHandler, initializationMask, Head, initializationAllFile);
        filePrintModMaskCow.join();
        filePrintMask.join();
        initializationFilePrinter(initializationAllFile, initializationTail);
        lk.unlock();
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void processFileAllInfoPrinter(FILE* file, char* str)
{
    unsigned __int8 j = 0xFF;
    processFileAllInfoMutex.lock();
    while (str[++j] != '\000')
    {
        if (str[j] == '|') { fprintf_s(file, "%c", ' '); j++; }
        fprintf_s(file, "%c", str[j]);
    }
    processFileAllInfoMutex.unlock();
}

void processFilePrinterMaskAllInfo(FILE* file, char* str)
{
    unsigned __int8 j = 0;
    unique_lock<mutex> lk(processFileAllInfoMutex);
    processFileAllInfoPrintCond.wait(lk, []()->bool {return processFileAllInfoPrintBool; });
    while (str[++j] != '\000')
    {
        if (str[j] == '|') { fprintf_s(file, "%c", ' '); j++; }
        fprintf_s(file, "%c", str[j]);
    }
    processFileAllInfoPrintBool = false;
    processFileAllInfoPrintCond.notify_all();
    j = 0;
    lk.unlock();
}

void processOutputAllInfoHandler(char* fileAllInfoOutputMask, Info block)
{
    unsigned __int8 count = 0, j = 0;
    unique_lock<mutex> lk(processOutputWorldHandlerMutex);
    thread gCHTimeStore, gCHCowsStore, gCHSStore, gCHMEStore, gCHVemidStore, gCHVimidStore, gCHGrassStore, gCHDieCowsStore;
    while (fileAllInfoOutputMask[j++] != '\n')
    {
        if (fileAllInfoOutputMask[j] == '|')
        {
            if (!count)
            {
                processFileAllInfoPrintCond.wait(lk, []()->bool {return !processFileAllInfoPrintBool; });
                thread gCHTime([fileAllInfoOutputMask, j, block]()->void {processInfoCharsHandler(fileAllInfoOutputMask, j, block.WorldInfo[0], (unsigned __int16)0x0001); });
                gCHTimeStore = move(gCHTime);
                count++;
            }
            else if (count == 1)
            {
                thread gCHCows([fileAllInfoOutputMask, j]()->void {processInfoCharsHandler(fileAllInfoOutputMask, j, COWS_COUNT, (unsigned __int16)0x0002); });
                gCHCowsStore = move(gCHCows);
                count++;
            }
            else if (count == 2)
            {
                thread gCHSS([fileAllInfoOutputMask, j, block]()->void {processInfoFloatCharsHandler(fileAllInfoOutputMask, j, block.CowInfo[0], (unsigned __int16)0x0004); });
                gCHSStore = move(gCHSS);
                count++;
            }
            else if (count == 3)
            {
                thread gCHME([fileAllInfoOutputMask, j, block]()->void {processInfoFloatCharsHandler(fileAllInfoOutputMask, j, block.CowInfo[1], (unsigned __int16)0x0008); });
                gCHMEStore = move(gCHME);
                count++;
            }
            else if (count == 4)
            {
                thread gCHVemid([fileAllInfoOutputMask, j, block]()->void {processInfoFloatCharsHandler(fileAllInfoOutputMask, j, block.CowInfo[2], (unsigned __int16)0x0010); });
                gCHVemidStore = move(gCHVemid);
                count++;
            }
            else if (count == 5)
            {
                thread gCHVimid([fileAllInfoOutputMask, j, block]()->void {processInfoFloatCharsHandler(fileAllInfoOutputMask, j, block.CowInfo[3], (unsigned __int16)0x0020); });
                gCHVimidStore = move(gCHVimid);
                count++;
            }
            else if (count == 6)
            {
                thread gCHGrass([fileAllInfoOutputMask, j, block]()->void {processInfoCharsHandler(fileAllInfoOutputMask, j, block.WorldInfo[1], (unsigned __int16)0x0040); });
                gCHGrassStore = move(gCHGrass);
                count++;
            }
            else
            {
                thread gCHDieCows([fileAllInfoOutputMask, j]()->void {processInfoCharsHandler(fileAllInfoOutputMask, j, COWS_AGE_DIE, (unsigned __int16)0x0080); });
                gCHDieCowsStore = move(gCHDieCows);
                count = 0;
            }
        }
    }
    gCHTimeStore.join();
    gCHCowsStore.join();
    gCHSStore.join();
    gCHMEStore.join();
    gCHVemidStore.join();
    gCHVimidStore.join();
    gCHGrassStore.join();
    gCHDieCowsStore.join();
    processHandlerSemaphoreInfoCond.wait(lk, []()->bool {return !processHandlerInfoSemaphore; });
    processHandlerInfoSemaphore = (unsigned __int16)0x00FF;
    processFileAllInfoPrintBool = true;
    processFileAllInfoPrintCond.notify_all();
    processHandlerInfoSemaphore = (unsigned __int16)0x00FF;
    lk.unlock();
}

void processFileAllInfoOutput(TileWorld* world, Cow* Head, FILE* info, unsigned __int32 time, char* fileAllInfoOutputHead, char* fileAllInfoOutputTail, char* fileAllInfoOutputMask)
{
    Info block;
    block.WorldInfo[0] = time;
    float* tempCowData = (float*)malloc(sizeof(float) * 4);
    unsigned __int32 grass = 0;
    for (unsigned __int8 i = 0; i < 4; i++)
    {
        tempCowData[i] = 0;
    }
    thread totalGrass([&grass, world]()->void {for (unsigned __int16 iterator = 0; iterator < LENGHT; iterator++)
    {
        grass += world[iterator].Grass;
    }});
    thread Smid([&tempCowData, Head]()mutable->void {while (Head != nullptr)
    {
        tempCowData[0] += (float)Head->Size;
        Head = Head->Next;
    }});
    thread MEmid([&tempCowData, Head]()mutable->void {while (Head != nullptr)
    {
        tempCowData[1] += (float)Head->MetabolismEfficiency;
        Head = Head->Next;
    }});
    thread Vemid([&tempCowData, Head]()mutable->void {while (Head != nullptr)
    {
        tempCowData[2] += (float)Head->Velocity;
        Head = Head->Next;
    }});
    thread Vimid([&tempCowData, Head]()mutable->void {while (Head != nullptr)
    {
        tempCowData[3] += (float)Head->Vision;
        Head = Head->Next;
    }});
    totalGrass.join();
    Smid.join();
    MEmid.join();
    Vemid.join();
    Vimid.join();
    for (unsigned __int8 i = 0; i < 4; i++)
    {
        tempCowData[i] /= (float)COWS_COUNT;
        block.CowInfo[i] = tempCowData[i];
    }
    block.WorldInfo[1] = grass;
    thread filePrintMask(processOutputAllInfoHandler, fileAllInfoOutputMask, block);
    thread filePrintModMask(processFilePrinterMaskAllInfo, info, fileAllInfoOutputMask);
    filePrintMask.join();
    filePrintModMask.join();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void iniReader()
{
    FILE* ini;
    fopen_s(&ini, "config.ini", "r");
    unsigned __int32 number = 0; unsigned __int16 number2 = 0; float number3 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%u", &number);
    TIME = number;
    number = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    SEASON_LENGHT = number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    SEASON = number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    LENGHT = number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    GROW_SPEED = number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    GRASS_START = number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    GRASS_MAX = number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    GRASS_RANGE_EXPANSION = number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    GRASS_EXPANSION_LIMIT = number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    if (number2 == 0)
    {
        number2 = 4;
    }
    COWS_COUNT = LENGHT / number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    CHANCE_NUMERATOR = number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    CHANCE_DENOMINATOR = number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    BREED_TYPE = number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    ATP_START = number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    RAW_EAT_START = number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%f", &number3);
    AGE_MIN = number3;
    number3 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%f", &number3);
    AGE_MULT = number3;
    number3 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%f", &number3);
    AGE_T_WEIGHT = number3;
    number3 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    BIRTH_COUNT = number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    TWCTE_ARCTIC[0] = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != ',');
    fscanf_s(ini, "%hu", &number2);
    TWCTE_ARCTIC[1] = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != ',');
    fscanf_s(ini, "%hu", &number2);
    TWCTE_ARCTIC[2] = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != ',');
    fscanf_s(ini, "%hu", &number2);
    TWCTE_ARCTIC[3] = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    TWCTE_MODERATE[0] = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != ',');
    fscanf_s(ini, "%hu", &number2);
    TWCTE_MODERATE[1] = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != ',');
    fscanf_s(ini, "%hu", &number2);
    TWCTE_MODERATE[2] = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != ',');
    fscanf_s(ini, "%hu", &number2);
    TWCTE_MODERATE[3] = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    TWCTE_EQUATOR[0] = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != ',');
    fscanf_s(ini, "%hu", &number2);
    TWCTE_EQUATOR[1] = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != ',');
    fscanf_s(ini, "%hu", &number2);
    TWCTE_EQUATOR[2] = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != ',');
    fscanf_s(ini, "%hu", &number2);
    TWCTE_EQUATOR[3] = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    ARCTIC_SOIL_RND = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != ',');
    fscanf_s(ini, "%hu", &number2);
    ARCTIC_SOIL_OFFSET = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    MODERATE_SOIL_RND = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != ',');
    fscanf_s(ini, "%hu", &number2);
    MODERATE_SOIL_OFFSET = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != '{');
    fscanf_s(ini, "%hu", &number2);
    EQUATOR_SOIL_RND = (unsigned __int8)number2;
    number2 = 0;
    while (fgetc(ini) != ',');
    fscanf_s(ini, "%hu", &number2);
    EQUATOR_SOIL_OFFSET = (unsigned __int8)number2;
    number2 = 0;
    fclose(ini);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    iniReader();
    {
        if (!LENGHT)
        {
            LENGHT = 40000;
        }
        if (!SEASON_LENGHT)
        {
            SEASON_LENGHT = 100;
        }
        if (!GROW_SPEED)
        {
            GROW_SPEED = 1;
        }
        if (!GRASS_MAX)
        {
            GRASS_MAX = 200;
        }
        if (!GRASS_EXPANSION_LIMIT)
        {
            GRASS_EXPANSION_LIMIT = 1;
        }
        if (!CHANCE_DENOMINATOR)
        {
            CHANCE_DENOMINATOR = 1;
        }
    }
    // Защита от нулевых значений

    thread processNotify([]()->void {while (true) { std::this_thread::sleep_for(std::chrono::milliseconds(500)); processPrintCowCond.notify_all(); processPrintCond.notify_all(); }});
    processNotify.detach();
    bool out = true;
    unsigned __int32 time = 0xFFFFFFFF;
    unsigned __int16 startCowsIterator = 0, * ptrStartCowsIterator = &startCowsIterator;
    TileWorld* world = (TileWorld*)malloc(sizeof(TileWorld) * LENGHT);
#ifndef NO_INFO_2
    FILE* initializationAllFile;
#endif // NO_INFO_2
#ifndef NO_INFO_1
    FILE* processAllFile;
#endif // NO_INFO_1
    FILE* infoAllFile;
#ifndef NO_INFO_2
    fopen_s(&initializationAllFile, "initialization.csv", "w+");
#endif //NO_INFO_2
    char processCSVName[16] = "process_0.csv", processCSVType[5] = ".csv";
    Cow* Head = nullptr, * Tail = nullptr;
    unsigned __int8 processCSVCounter = 1;
    char initializationWorldHead[129] = "Initialition World\n/////////////////////////////////////////////////////\n|   id   | Grass | GrowLimit | TileType | TileClimat |\n", initializationWorldTail[56] = "/////////////////////////////////////////////////////\n\n", initializationCowHead[138] = "Initialition Cow\n///////////////////////////////////////////////////////////\n|   id   |   Age  |  ATP |  Place |  S | ME |  C | Ve| Vi |\n", initializationWorldMask[56] = "|        |       |           |          |            |\n", initializationCowMask[62] = "|        |        |      |        |    |    |    |    |    |\n", initializationCowTail[62] = "///////////////////////////////////////////////////////////\n\n";
    char processCowHead[145] = "Process Cow\n///////////////////////////////////////////////////////////\n|   id   |   Age  |  ATP |  Place |  S | ME |  C | Ve| Vi |\n", processCowTail[62] = "///////////////////////////////////////////////////////////\n\n", processWorldHead[124] = "Process World\n/////////////////////////////////////////////////////\n|   id   | Grass | GrowLimit | TileType | TileClimat |\n", processWorldTail[56] = "/////////////////////////////////////////////////////\n\n", processWorldMask[56] = "|        |       |           |          |            |\n", processCowMask[62] = "|        |        |      |        |    |    |    |    |    |\n";
    char fileAllInfoOutputHead[183] = "Process Genereal Output\n/////////////////////////////////////////////////////////////////////////////\n|  Time  |    Cows    | Smid |  MEmid |  Vemid |  Vimid |   Grass  |   ADCow  |\n", fileAllInfoOutputTail[85] = "//////////////////////////////////////////////////////////////////////////////////\n\n", fileAllInfoOutputMask[84] = "|        |               |      |        |        |        |          |          |\n";
    thread fillerWorld([world]()->void {for (unsigned __int16 i = 0; i < LENGHT; i++)
    {
        world[i].Grass = GRASS_START;
        world[i].GrassExpansionLimit = GRASS_EXPANSION_LIMIT;
        world[i].Head = nullptr;
    }});
    thread fillerWorldTileClimat([world]()->void {unsigned __int16 j = 0; unsigned __int8 tileType = 0; for (unsigned __int16 i = 0; i < LENGHT; i++)
    {
        if (i < (LENGHT * 3 / 40))
        {
            if (!j)
            {
                tileType = rand() % EQUATOR_SOIL_RND + EQUATOR_SOIL_OFFSET;
            }
            world[i].TileClimat = TWCTE_EQUATOR[SEASON];
            world[i].TileType = tileType;
        }
        else if (i >= (LENGHT * 3 / 40) && i < (LENGHT * 7 / 40))
        {
            if (!j)
            {
                tileType = rand() % MODERATE_SOIL_RND + MODERATE_SOIL_OFFSET;
            }
            world[i].TileClimat = TWCTE_MODERATE[SEASON];
            world[i].TileType = tileType;
        }
        else if (i >= (LENGHT * 7 / 40) && i < (LENGHT * 13 / 40))
        {
            if (!j)
            {
                tileType = rand() % ARCTIC_SOIL_RND + ARCTIC_SOIL_OFFSET;
            }
            world[i].TileClimat = TWCTE_ARCTIC[SEASON];
            world[i].TileType = tileType;
        }
        else if (i >= (LENGHT * 13 / 40) && i < (LENGHT * 17 / 40))
        {
            if (!j)
            {
                tileType = rand() % MODERATE_SOIL_RND + MODERATE_SOIL_OFFSET;
            }
            world[i].TileClimat = TWCTE_MODERATE[SEASON];
            world[i].TileType = tileType;
        }
        else if (i >= (LENGHT * 17 / 40) && i < (LENGHT * 20 / 40))
        {
            if (!j)
            {
                tileType = rand() % EQUATOR_SOIL_RND + EQUATOR_SOIL_OFFSET;
            }
            world[i].TileClimat = TWCTE_EQUATOR[SEASON];
            world[i].TileType = tileType;
        }
        else if (i >= (LENGHT * 20 / 40) && i < (LENGHT * 23 / 40))
        {
            if (!j)
            {
                tileType = rand() % EQUATOR_SOIL_RND + EQUATOR_SOIL_OFFSET;
            }
            world[i].TileClimat = TWCTE_EQUATOR[(2 + SEASON) % 4];
            world[i].TileType = tileType;
        }
        else if (i >= (LENGHT * 23 / 40) && i < (LENGHT * 27 / 40))
        {
            if (!j)
            {
                tileType = rand() % MODERATE_SOIL_RND + MODERATE_SOIL_OFFSET;
            }
            world[i].TileClimat = TWCTE_MODERATE[(2 + SEASON) % 4];
            world[i].TileType = tileType;
        }
        else if (i >= (LENGHT * 27 / 40) && i < (LENGHT * 33 / 40))
        {
            if (!j)
            {
                tileType = rand() % ARCTIC_SOIL_RND + ARCTIC_SOIL_OFFSET;
            }
            world[i].TileClimat = TWCTE_ARCTIC[(2 + SEASON) % 4];
            world[i].TileType = tileType;
        }
        else if (i >= (LENGHT * 33 / 40) && i < (LENGHT * 37 / 40))
        {
            if (!j)
            {
                tileType = rand() % MODERATE_SOIL_RND + MODERATE_SOIL_OFFSET;
            }
            world[i].TileClimat = TWCTE_MODERATE[(2 + SEASON) % 4];
            world[i].TileType = tileType;

        }
        else
        {
            if (!j)
            {
                tileType = rand() % EQUATOR_SOIL_RND + EQUATOR_SOIL_OFFSET;
            }
            world[i].TileClimat = TWCTE_EQUATOR[(2 + SEASON) % 4];
            world[i].TileType = tileType;
        }
        if (!j)
        {
            j = (rand() % 39) + 1;
        }
        j--;
    }});
#ifndef NO_INFO_2
    thread fileFillerWorld(initializationFilePrintWorld, initializationAllFile, initializationWorldHead, initializationWorldMask, initializationWorldTail, world);
    thread initializationNotify([&out]()->void {while (out) { std::this_thread::sleep_for(std::chrono::milliseconds(500)); initializationPrintCond.notify_all(); }});
    initializationNotify.detach();
#endif // !NO_INFO_2
    thread fillerCowExistStore;
    fillerWorld.join();
    if (COWS_COUNT)
    {
        Head = (Cow*)malloc(sizeof(Cow));
        Head->Position = rand() % LENGHT;
        Head->Age = AGE_START;
        Head->MovePos = Head->Position;
        Head->LiveTime = rand() % 2 + 2;
        Head->ATP = ATP_START;
        Head->RawEat = RAW_EAT_START;
        Head->Size = rand() % 2 + 1;
        Head->ATPPool = ATP_POOL * Head->Size;
        Head->Catabolism = CATABOLISM * Head->Size;
        Head->EatPool = EAT_POOL * Head->Size;
        Head->MetabolismEfficiency = METABOLISM_EFFICIENCY;
        Head->RawEatPool = RAW_EAT_POOL * Head->Size;
        Head->Velocity = rand() % 2 + 1;
        Head->Vision = rand() % 3 + 2;
        Head->Estrus = 0;
        Head->Next = nullptr;
        Head->Prev = nullptr;
        Tail = Head;
        thread fillerCowExist([Head, &Tail, &world]()->void
            {
                Cow* tempCow = Head;
                world[Head->Position].Head = (TileCow*)malloc(sizeof(TileCow));
                world[Head->Position].Head->Now = Head;
                world[Head->Position].Head->Next = nullptr;
                for (unsigned __int16 i = 1; i < COWS_COUNT; i++)
                {
                    Cow* newCow = (Cow*)malloc(sizeof(Cow));
                    newCow->Position = rand() % LENGHT;
                    TileCow* tempTileCow = world[newCow->Position].Head, * tempPrevTileCow = nullptr;
                    while (tempTileCow != nullptr)
                    {
                        tempPrevTileCow = tempTileCow;
                        tempTileCow = tempTileCow->Next;
                    }
                    if (tempPrevTileCow == nullptr)
                    {
                        world[newCow->Position].Head = (TileCow*)malloc(sizeof(TileCow));
                        world[newCow->Position].Head->Now = newCow;
                        world[newCow->Position].Head->Next = nullptr;
                    }
                    else
                    {
                        tempPrevTileCow->Next = (TileCow*)malloc(sizeof(TileCow));
                        tempPrevTileCow->Next->Now = newCow;
                        tempPrevTileCow->Next->Next = nullptr;
                    }
                    newCow->Age = AGE_START;
                    newCow->MovePos = newCow->Position;
                    newCow->LiveTime = rand() % 2 + 1;
                    newCow->ATP = ATP_START;
                    newCow->RawEat = RAW_EAT_START;
                    newCow->Size = rand() % 2 + 1;
                    newCow->ATPPool = ATP_POOL * newCow->Size;
                    newCow->Catabolism = CATABOLISM * newCow->Size;
                    newCow->EatPool = EAT_POOL * newCow->Size;
                    newCow->MetabolismEfficiency = METABOLISM_EFFICIENCY;
                    newCow->RawEatPool = RAW_EAT_POOL * newCow->Size;
                    newCow->Velocity = rand() % 2 + 1;
                    newCow->Vision = rand() % 3 + 2;
                    newCow->Estrus = 0;
                    newCow->Next = nullptr;
                    newCow->Prev = tempCow;
                    tempCow->Next = newCow;
                    tempCow = newCow;
                }
                Tail = tempCow;
                thread cowSemaphoreAdd(initializationCowSemaphoreInc);
                cowSemaphoreAdd.join();
            });
        fillerCowExistStore = move(fillerCowExist);
    }
    else
    {
        thread cowSemaphoreAdd(initializationCowSemaphoreInc);
        cowSemaphoreAdd.join();
    }
#ifndef NO_INFO_2
    thread fileFillerCow(initializationFilePrintCow, initializationAllFile, initializationCowHead, initializationCowMask, initializationCowTail, Head);
    fopen_s(&processAllFile, processCSVName, "w+");
#endif // !NO_INFO_2
    fopen_s(&infoAllFile, "info.csv", "w+");
    thread filePrintHead(processFileAllInfoPrinter, infoAllFile, fileAllInfoOutputHead);
    fillerWorldTileClimat.join();
    if (fillerCowExistStore.joinable())
    {
        fillerCowExistStore.join();
    }
#ifndef NO_INFO_2
    fileFillerWorld.join();
    fileFillerCow.join();
    out = false;
    fclose(initializationAllFile);
#endif // !NO_INFO_2
    filePrintHead.join();
    while (++time < TIME)
    {
        printf_s("%u\n\n", time);
        thread cowHandler([&world, &Head, &ptrStartCowsIterator, &Tail]()->void
            {
                unique_lock<mutex> lk(processCowHandlerMutex);
                processPrintCowCond.wait(lk, []()->unsigned __int8 {return !(cowHandlerBool xor 0b00000000); });
                cowHandler(world, &Head, &Tail);
                lk.unlock();
            });
        thread tileHandler([&world, &time]()->void
            {
                unique_lock<mutex> lk(processWorldHandlerMutex);
                processPrintCowCond.wait(lk, []()->unsigned __int8 {return !((cowHandlerBool & 0b00000001) xor 0b00000001); });
                tileHandler(world, &time);
                lk.unlock();
            });
        unique_lock<mutex> lk2(processOutputMutex);
        cowHandler.join();
        tileHandler.join();
        processPrintCowCond.wait(lk2, []()->unsigned __int8
            {
                return !((cowHandlerBool & 0b00000011) xor 0b00000011);
            });
        processCowPrintSemaphoreContinue = true;
#ifndef NO_INFO_1
        thread processFileFillerWorld(processFilePrintWorld, processFilePrinter, processOutputWorldHandler, processAllFile, processWorldHead, processWorldMask, processWorldTail, world);
        thread processFileFillerCow(processFilePrintCow, processFilePrinter, processOutputCowHandler, processAllFile, processCowHead, processCowMask, processCowTail, Head);
#endif // !NO_INFO_1
        thread worldInfo(processFileAllInfoOutput, world, Head, infoAllFile, time, fileAllInfoOutputHead, fileAllInfoOutputTail, fileAllInfoOutputMask);
        lk2.unlock();
#ifdef NO_INFO_1
        cowHandlerBoolXor(0b00000011);
#endif // !NO_INFO_1
#ifndef NO_INFO_1
        processFileFillerWorld.join();
        processFileFillerCow.join();
#endif // !NO_INFO_1
        worldInfo.join();
#ifndef NO_INFO_1
        if (!((time % 50) - 49))
        {
            fclose(processAllFile);
            processCSVHandler(processCSVCounter, processCSVName, processCSVType);
            processCSVCounter++;
            fopen_s(&processAllFile, processCSVName, "w+");
        }
#endif // !NO_INFO_1
    }
    thread filePrintTail(processFileAllInfoPrinter, infoAllFile, fileAllInfoOutputTail);
    filePrintTail.join();
#ifndef NO_INFO_2
    fclose(processAllFile);
#endif // !NO_INFO_2
    fclose(infoAllFile);
    if (Head)
    {
        Cow* freeCow = Head;
        while (freeCow != Tail)
        {
            freeCow = freeCow->Next;
            free(freeCow->Prev);
        }
        free(freeCow);
    }
    free(world);
}