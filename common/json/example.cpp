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


  constexpr const char id_pippo[] = "pippo";
  constexpr const char id_pluto[] = "pluto";
  constexpr const char* const plutoKeys[] = {Pluto::id_pippo, Pluto::id_pluto};

  class Pluto : public ToJson<Map<plutoKeys[0], int>, Map<plutoKeys[1], double>>{
  public:
    Pluto(){}
    Pluto(Pluto& p){
      this->pippo = p.getPippo();
      this->pluto = p.getPluto();
    }
    Pluto(const int pippo, const double pluto){
      this->pippo = pippo;
      this->pluto = pluto;
    }

    int getPippo(){ return this->pippo; }
    double getPluto(){ return this->pluto; }
    void setPippo(const int& pippo){ this->pippo = pippo; }
    void setPluto(const double& pluto){ this->pluto = pluto; }

  protected:
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
  //to_json(j, p);
  std::cout << j.dump(2) << std::endl;
  Json::Pluto q;
  q.fromJson(j);
  std::cout << "q: pippo: " << q.getPippo() << "; pluto: " << q.getPluto() << std::endl;
}
