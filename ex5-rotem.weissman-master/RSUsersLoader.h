#ifndef USERFACTORY_H
#define USERFACTORY_H
#include "RecommenderSystem.h"
#include "RSUser.h"
#include "Movie.h"
#include "string"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>


class RSUsersLoader
{
private:


public:
    RSUsersLoader() = delete;
    /**
     *
     * loads users by the given format with their movie's ranks
     * @param users_file_path a path to the file of
     * the users and their movie ranks
     * @param rs RecommendingSystem for the Users
     * @return vector of the users created according to the file
     */
    static std::vector<RSUser> create_users_from_file(const std::string&
    users_file_path, std::unique_ptr<RecommenderSystem> rs) noexcept(false);

};


#endif //USERFACTORY_H
//testing