/*!
 \file ToJson.hpp
 \author Enrico Magnago
 \date 08.08.2017
 \brief base class to class serialization in JSON format
*/

#ifndef __TO_JSON_HPP__
#define __TO_JSON_HPP__

#include "json.hpp" // from: https://github.com/nlohmann/json


namespace Json{
  /*!
    \brief class used to define single mapping: char* -> type
  */
  template <const char* _key, typename _T>
  struct Map{
    static constexpr const char* key = _key;
    typedef _T Type;
  };


  template <typename ...Types>
  class ToJson{
  public:
    void toJson(nlohmann::json& j) const{}
    void fromJson(const nlohmann::json& j){}
  protected:
    virtual void set(const char* const key, void* const item) = 0;
    virtual const void* get(const char* const key) const = 0;
    //inline static constexpr unsigned short int getJsonKeysNumber();
  };


  template < typename T, typename ...Types >
  class ToJson<T, Types...> : virtual public ToJson<Types ...>
  {
  public:
    constexpr void toJson(nlohmann::json& j) const;
    constexpr void fromJson(const nlohmann::json& j);
    /*inline static constexpr unsigned short int getJsonKeysNumber(){
      return index + 1;
      }
      static constexpr unsigned short int index = sizeof...(Types);*/
  };




  /* methods to allow (de)serialization of ToJson objects */
  template <typename ... Ts>
  constexpr void to_json(nlohmann::json& j, const ToJson<Ts...>& obj){
    obj.toJson(j);
  }

  template <typename ... Ts>
  constexpr void from_json(const nlohmann::json& j, ToJson<Ts...>& obj){
    obj.fromJson(j);
  }

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

#endif /* __TO_JSON_HPP__ */
