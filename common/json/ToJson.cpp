/*!
 \file ToJson.cpp
 \author Enrico Magnago
 \date 08.08.2017
 \brief base class to class serialization in JSON format
*/

#include <iostream>

#include "ToJson.hpp"

#include <type_traits>
#include <boost/type_index.hpp>
#include <string>


namespace Json{

  constexpr char id_pippo[] = "pippo";
  constexpr char id_pluto[] = "pluto";

  class Pluto : public ToJson<Map<id_pippo, int>, Map<id_pluto, double>>{

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

    void set(const char* const key, void* const item){
      if(strcmp(key, id_pippo) == 0){
        this->pippo = *(static_cast<int* const>(item));
        return;
      }
      if(strcmp(key, id_pluto) == 0){
        this->pluto = *(static_cast<double* const>(item));
        return;
      }
      std::cerr << "unreachable code: Pluto::set(" << key <<
        ");" << std::endl;
      exit(1);
    }

    const void* get(const char* const key) const{
      if(strcmp(key, id_pippo) == 0)
        return &(this->pippo);

      if(strcmp(key, id_pluto) == 0)
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
  to_json(j, p);
  std::cout << j.dump(2) << std::endl;
  Json::Pluto q;
  q.fromJson(j);
  std::cout << "q: pippo: " << q.getPippo() << "; pluto: " << q.getPluto() << std::endl;
}

namespace Json{


  template <typename T, typename ...Types >
  constexpr void ToJson<T, Types...>::toJson(nlohmann::json& j) const{
    j[T::key] = *(static_cast<const typename T::Type *>(this->get(T::key)));
    ToJson<Types...>::toJson(j);
  }


  template <typename T, typename ...Types >
  constexpr void ToJson<T, Types...>::fromJson(const nlohmann::json& j){
    typename T::Type alias = j.at((const char* const)T::key).get<typename T::Type>();
    this->set(T::key, &alias);
    ToJson<Types...>::fromJson(j);
  }
}
