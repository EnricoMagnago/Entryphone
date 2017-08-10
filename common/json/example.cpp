/*
  Small example of ToJson usage:
  Pluto must be declared in Json namespace
  Pluto inherits from ToJson declaring his Mappings: key -> value type
  Pluto implements a generic get and set for the keys.

  Issues:
  1. ids must be declared globally inside the namespace,
     in these example only the array is used inside the code
     in these use case maybe it can be nicer to get an index instead of a string in the set and get functions (can be done)

  2. at the moment the library functions to_json and from_json are not working, compile time error, thus these classes
     can not be (de)serialized using the assignment operator.
*/


#include <iostream>

#include "ToJson.hpp"

namespace Json{

  /*
    keys used in the json serialization of Pluto,
    by splitting declaration and definition these strings could be placed inside as
    static constexpr const char [].
   */
  constexpr const char id_pippo[] = "pippo";
  constexpr const char id_pluto[] = "pluto";
  constexpr const char* const plutoKeys[] = {id_pippo, id_pluto};

  class Pluto : public ToJson<Map<plutoKeys[0], int>, Map<plutoKeys[1], double>>{
  public:
    /*
      implement default, copy and move constructors
      would be nice to implement copy and move assignment too.
     */
    Pluto(){}
    Pluto(Pluto& p){
      this->pippo = p.getPippo();
      this->pluto = p.getPluto();
    }
    Pluto(Pluto&& p){
      this->pippo = std::move(p.getPippo());
      this->pluto = std::move(p.getPluto());
    }
    Pluto(const int pippo, const double pluto){
      this->pippo = pippo;
      this->pluto = pluto;
    }
    /*
      usual get and set methods.
     */
    int getPippo(){ return this->pippo; }
    double getPluto(){ return this->pluto; }
    void setPippo(const int& pippo){ this->pippo = pippo; }
    void setPluto(const double& pluto){ this->pluto = pluto; }


  protected:
    /*
      set takes a key and an item which is guarantee to be
      of the correspondent type of key, this method
      must set the inner field of these appropriately
      by taking the data from item.

      with respect to issue 2: the library could be implemented so that
      these methods takes an integer index instead of a string key.
     */
    void set(const char* const key, void* const item){
      if(strcmp(key, plutoKeys[0]) == 0){
        this->pippo = *(static_cast<int* const>(item));
        return;
      }
      if(strcmp(key, plutoKeys[1]) == 0){
        this->pluto = *(static_cast<double* const>(item));
        return;
      }
      std::cerr << "unreachable code: Pluto::set(" << key <<
        ");" << std::endl;
      exit(1);
    }

    /*
      get takes a key and return a pointer to an item which must be
      of the correspondent type of key.
      this method must return the address of the inner field corresponding to the
      given key.

      with respect to issue 2: the library could be implemented so that
      these methods takes an integer index instead of a string key.
     */
    const void* get(const char* const key) const{
      if(strcmp(key, plutoKeys[0]) == 0)
        return &(this->pippo);

      if(strcmp(key, plutoKeys[1]) == 0)
        return &(this->pluto);

      std::cerr << "unreachable code: Pluto::set(" << key <<
        ");" << std::endl;
      exit(1);
    }

  private:
    int pippo;
    double pluto;
  };

}

int main(){
  Json::Pluto p(2, 3.14);
  nlohmann::json j;
  p.toJson(j);
  /* decommenting one these line causes a compile time error..
     still trying to figure out exactly what the problem is.
   */
  //to_json(j, p);
  std::cout << j.dump(2) << std::endl;
  Json::Pluto q;
  /*
    Trying to deserialize from an empty json leads to an unpleasant error
    at runtime.
   */
  q.fromJson(j);
  /* decommenting one these line causes a compile time error..
     still trying to figure out exactly what the problem is.
     Same issue as to_json.
   */
  //from_json(j, p);
  std::cout << "q: pippo: " << q.getPippo() << "; pluto: " << q.getPluto() << std::endl;
}
