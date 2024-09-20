#include <vector>
#include <list>
#include <set>
#include <string>
#include <iostream>
#include <iomanip>
#include "imdb.h"
#include "path.h"
using namespace std;

/**
 * Using the specified prompt, requests that the user supply
 * the name of an actor or actress.  The code returns
 * once the user has supplied a name for which some record within
 * the referenced imdb existsif (or if the user just hits return,
 * which is a signal that the empty string should just be returned.)
 *
 * @param prompt the text that should be used for the meaningful
 *               part of the user prompt.
 * @param db a reference to the imdb which can be used to confirm
 *           that a user's response is a legitimate one.
 * @return the name of the user-supplied actor or actress, or the
 *         empty string.
 */

static string promptForActor(const string& prompt, const imdb& db)
{
  string response;
  while (true) {
    cout << prompt << " [or <enter> to quit]: ";
    getline(cin, response);
    if (response == "") return "";
    vector<film> credits;
    if (db.getCredits(response, credits)) return response;
    cout << "We couldn't find \"" << response << "\" in the movie database. "
	 << "Please try again." << endl;
  }
}

/**
 * Serves as the main entry point for the six-degrees executable.
 * There are no parameters to speak of.
 *
 * @param argc the number of tokens passed to the command line to
 *             invoke this executable.  It's completely ignored
 *             here, because we don't expect any arguments.
 * @param argv the C strings making up the full command line.
 *             We expect argv[0] to be logically equivalent to
 *             "six-degrees" (or whatever absolute path was used to
 *             invoke the program), but otherwise these are ignored
 *             as well.
 * @return 0 if the program ends normally, and undefined otherwise.
 */

void coutPath(path p){
  p.reverse();
  while(p.getLength() != 0){
    cout << p.getLastPlayer() << " ";
    p.undoConnection();
  }

}



int main(int argc, const char *argv[])
{
  imdb db(determinePathToData(argv[1])); // inlined in imdb-utils.h
  if (!db.good()) {
    cout << "Failed to properly initialize the imdb database." << endl;
    cout << "Please check to make sure the source files exist and that you have permission to read them." << endl;
    return 1;
  }



  while (true) {
    bool ans = false;
    string source = promptForActor("Actor or actress", db);
    if (source == "") break;
    string target = promptForActor("Another actor or actress", db);
    if (target == "") break;
    if (source == target) {
      cout << "Good one.  This is only interesting if you specify two different people." << endl;
    } else {
      // replace the following line by a call to your generateShortestPath routine... 
      list<path> paths;
      set<string> prevActors;
      set<film> prevFilms;

      path p(source);
      paths.push_back(p);
      while(true){
        if(paths.empty()) break;
        path currP = paths.front();
        paths.pop_front();
        if(currP.getLength() > 5) break;
        vector<film> movies;
        db.getCredits(currP.getLastPlayer(),movies);
        for(film f : movies){
          if(prevFilms.find(f) != prevFilms.end()) continue; //skip if movie is visited
          prevFilms.insert(f);
          vector<string> players;
          db.getCast(f,players);
          for(string player : players){
            if(prevActors.find(player) != prevActors.end()) continue; // skip if actor is visited
            path newP = currP;
            newP.addConnection(f,player);
            if(player == target){
              newP.print();
              ans = true;
              break;
            }
            paths.push_back(newP);
          }
          if(ans) break;
        }
        if(ans) break;

      }
      if(!ans) cout << "No path between those two people could be found." << endl;
    }
  }
  
  cout << "Thanks for playing!" << endl;
  return 0;
}

