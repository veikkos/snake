#include "ai.h"

Ai::Ai(){

}

Ai::~Ai(){

}

void Ai::GetDir(Snake *ai_snake, Eatable *cur_eatable){

    Direction old_dir = ai_snake->GetDir(), new_dir;

    new_dir = old_dir;

    // Get direction based on the eatable...
    if((ai_snake->GetDir() == right) || (ai_snake->GetDir() == left)){

        if(ai_snake->GetPosY(0) < cur_eatable->GetPosY(0))
            new_dir = down;
        else if(ai_snake->GetPosY(0) > cur_eatable->GetPosY(0) )
            new_dir = up;

    }else if((ai_snake->GetDir() == up) || (ai_snake->GetDir() == down)){

        if(ai_snake->GetPosX(0) < cur_eatable->GetPosX(0))
            new_dir = right;
        else if(ai_snake->GetPosX(0) > cur_eatable->GetPosX(0))
            new_dir = left;
    }

    // ...and check that snake doesn't crash to itself on next block.
    if(CollisionIn(ai_snake, new_dir) == false){
        ai_snake->SetDir(new_dir);
        return;
    }

    // Prioritize the first two options with the distance to
    // the eatable.
    switch(new_dir){

        case right:
        {
            if(ai_snake->GetPosY(0) > cur_eatable->GetPosY(0)){
                if(CollisionIn(ai_snake, up) == false){
                    ai_snake->SetDir(up);
                    return;
                }else if(CollisionIn(ai_snake, down) == false){
                    ai_snake->SetDir(down);
                    return;
                }
            }else{
                if(CollisionIn(ai_snake, down) == false){
                    ai_snake->SetDir(down);
                    return;
                }else if(CollisionIn(ai_snake, up) == false){
                    ai_snake->SetDir(up);
                    return;
                }
            }

            if(CollisionIn(ai_snake, left) == false){
                ai_snake->SetDir(left);
                return;
            }

            break;
        }
        case left:
        {
            if(ai_snake->GetPosY(0) > cur_eatable->GetPosY(0)){
                if(CollisionIn(ai_snake, up) == false){
                    ai_snake->SetDir(up);
                    return;
                }else if(CollisionIn(ai_snake, down) == false){
                    ai_snake->SetDir(down);
                    return;
                }
            }else{
                if(CollisionIn(ai_snake, down) == false){
                    ai_snake->SetDir(down);
                    return;
                }else if(CollisionIn(ai_snake, up) == false){
                    ai_snake->SetDir(up);
                    return;
                }
            }

            if(CollisionIn(ai_snake, right) == false){
                ai_snake->SetDir(right);
                return;
            }

            break;
        }
        case down:
        {
            if(ai_snake->GetPosX(0) > cur_eatable->GetPosX(0)){
                if(CollisionIn(ai_snake, left) == false){
                    ai_snake->SetDir(left);
                    return;
                }else if(CollisionIn(ai_snake, right) == false){
                    ai_snake->SetDir(right);
                    return;
                }
            }else{
                if(CollisionIn(ai_snake, right) == false){
                    ai_snake->SetDir(right);
                    return;
                }else if(CollisionIn(ai_snake, left) == false){
                    ai_snake->SetDir(left);
                    return;
                }
            }

            if(CollisionIn(ai_snake, up) == false){
                ai_snake->SetDir(up);
                return;
            }

            break;
        }
        case up:
        {
            if(ai_snake->GetPosX(0) > cur_eatable->GetPosX(0)){
                if(CollisionIn(ai_snake, left) == false){
                    ai_snake->SetDir(left);
                    return;
                }else if(CollisionIn(ai_snake, right) == false){
                    ai_snake->SetDir(right);
                    return;
                }
            }else{
                if(CollisionIn(ai_snake, right) == false){
                    ai_snake->SetDir(right);
                    return;
                }else if(CollisionIn(ai_snake, left) == false){
                    ai_snake->SetDir(left);
                    return;
                }
            }

            if(CollisionIn(ai_snake, down) == false){
                ai_snake->SetDir(down);
                return;
            }

            break;
        }
    }
}

bool Ai::CollisionIn(Snake *ai_snake, Direction dir){

    int i;

    for(i=1; i<ai_snake->GetLength(); i++){

        switch(dir){

            case right:
            {
                if((ai_snake->GetPosX(0) + GRID_SIZE) == ai_snake->GetPosX(i)){

                    if(ai_snake->GetPosY(0) == ai_snake->GetPosY(i)){
                        return true;
                    }
                }

                break;
            }
            case left:
            {
                if((ai_snake->GetPosX(0) - GRID_SIZE) == ai_snake->GetPosX(i)){

                    if(ai_snake->GetPosY(0) == ai_snake->GetPosY(i)){
                        return true;
                    }
                }

                break;
            }
            case down:
            {
                if((ai_snake->GetPosY(0) + GRID_SIZE) == ai_snake->GetPosY(i)){

                    if(ai_snake->GetPosX(0) == ai_snake->GetPosX(i)){
                        return true;
                    }
                }

                break;
            }
            case up:
            {
                if((ai_snake->GetPosY(0) - GRID_SIZE) == ai_snake->GetPosY(i)){

                    if(ai_snake->GetPosX(0) == ai_snake->GetPosX(i)){
                        return true;
                    }
                }

                break;
            }
        }
    }

    return false;
}
