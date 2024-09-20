using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "imdb.h"

const char *const imdb::kActorFileName = "actordata";
const char *const imdb::kMovieFileName = "moviedata";




imdb::imdb(const string& directory)
{
  const string actorFileName = directory + "/" + kActorFileName;
  const string movieFileName = directory + "/" + kMovieFileName;
  
  actorFile = acquireFileMap(actorFileName, actorInfo);
  movieFile = acquireFileMap(movieFileName, movieInfo);
}

bool imdb::good() const
{
  return !( (actorInfo.fd == -1) || 
	    (movieInfo.fd == -1) ); 
}

char* imdb::getActorPointerById(int id) const{
  int offset = *((int*)actorFile + 1 + id);
  //cout<< offset<<endl;
  char* pointer = (char*)actorFile + offset;
  return pointer;
}

string imdb::getActorNameById(int id) const{
  int offset = *((int*)actorFile + 1 + id);
  return getActorNameByOffset(offset);
}

string imdb::getActorNameByOffset(int offset) const{
  
  string name = "";
  //cout<< offset<<endl;
  char* pointer = (char*)actorFile + offset;
  char c = *pointer;
  while (c!= '\0')
  {
    name += c;
    pointer++;
    c = *pointer;
  }
  return name;
}


int imdb::getActorIdByName(string player) const{
  int actorAmount = *(int *)(actorFile);
  int start = 0;
  int end = actorAmount-1;
  while (start <= end)
  {
    int currId = start + (end - start)/2;
    string currActor = getActorNameById(currId);
    //cout<< start << " " << end<<" " << currId << " " << currActor<<endl;
    if(player == currActor) return currId;
    if(currActor < player) start = currId + 1;
    else end = currId - 1;
  }
  return -1;
}

  film imdb::getFilmByOffset(int offset) const{
    film f;
    string name;
    char* pointer = (char*)movieFile + offset;
    char c = *pointer;
    while (c!= '\0')
    {
      name += c;
      pointer++;
      c = *pointer;
    }
    int year = *(++pointer);
    f.title = name;
    f.year = 1900 + year;
    return f;
  }


  film imdb::getFilmById(int id) const{
    int offset = *((int*)movieFile + 1 + id);
    //cout << "offset " << offset <<endl;
    return getFilmByOffset(offset);
  }
  int imdb::getFilmIdByFilm(film f) const{
    int filmAmount = *(int *)(movieFile);
    //cout<<"amount "<<filmAmount<<endl;
    int start = 0;
    int end = filmAmount-1;
    while (start <= end)
    {
      int currId = start + (end - start)/2;
      film currFilm = getFilmById(currId);
      //cout<<start<<" "<<end << " " <<currFilm.title << " "<<currFilm.year<<endl;
      //cout<< start << " " << end<<" " << currId << " " << currActor<<endl;
      if(f == currFilm) return currId;
      if(currFilm < f) start = currId + 1;
      else end = currId - 1;
    }
    return -1;
  }
  char* imdb::getFilmPointerById(int id) const{
      int offset = *((int*)movieFile + 1 + id);
      //cout<< offset<<endl;
      char* pointer = (char*)movieFile + offset;
      return pointer;
  }

// you should be implementing these two methods right here... 
bool imdb::getCredits(const string& player, vector<film>& films) const { 
  //cout<< actorAmount<<endl;

  //find the id of the actor with binnary search
  int id = getActorIdByName(player);
  if(id == -1) return false;
  char* actorPointer = getActorPointerById(id);
  int len = player.length();

  short* moiveAmountPointer = (short *)(actorPointer + len + 1 + (len % 2 == 0));
  int moiveAmount = *moiveAmountPointer; // get the acmount of movies a player has played in
  int* moviePointer = (int *)(moiveAmountPointer + 1 + !(((len + 1 + (len % 2 == 0)) + 2) % 4 == 0));

  for(int i = 0; i < moiveAmount; i++){
    //cout<<getFilmByOffset(*(moviePointer+i)).title <<" "<<getFilmByOffset(*(moviePointer+i)).year<<endl;
    films.push_back(getFilmByOffset(*(moviePointer+i)));
  }
  return true; 

  }
bool imdb::getCast(const film& movie, vector<string>& players) const { 
  //cout<<"asdsad"<<endl;
  int id = getFilmIdByFilm(movie);
  //cout<< id;
  if(id == -1) return false;
  char* moviePointer = getFilmPointerById(id);
  int len = movie.title.length();

  short* actorAmountPointer = (short *)(moviePointer + len + 2 + !(len % 2 == 0));
  int actorAmount = *actorAmountPointer;
  int* actorPointer =(int *)(actorAmountPointer + 1 + !(((len + 2 + !(len % 2 == 0) + 2) % 4)== 0));

  for(int i = 0; i < actorAmount; i++){
    //cout<<getActorNameByOffset(*(actorPointer+i))<<endl;
    players.push_back(getActorNameByOffset(*(actorPointer+i)));
  }

  return true; 
  
  }


imdb::~imdb()
{
  releaseFileMap(actorInfo);
  releaseFileMap(movieInfo);
}

// ignore everything below... it's all UNIXy stuff in place to make a file look like
// an array of bytes in RAM.. 
const void *imdb::acquireFileMap(const string& fileName, struct fileInfo& info)
{
  struct stat stats;
  stat(fileName.c_str(), &stats);
  info.fileSize = stats.st_size;
  info.fd = open(fileName.c_str(), O_RDONLY);
  return info.fileMap = mmap(0, info.fileSize, PROT_READ, MAP_SHARED, info.fd, 0);
}

void imdb::releaseFileMap(struct fileInfo& info)
{
  if (info.fileMap != NULL) munmap((char *) info.fileMap, info.fileSize);
  if (info.fd != -1) close(info.fd);
}
