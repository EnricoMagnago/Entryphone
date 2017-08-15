/*!
 \file Json.hpp
 \author Enrico Magnago
 \date 08.08.2017
 \brief base class to class serialization in JSON format

 User of these header must create a class that inherits from Json.
 In particular the Json class template args must be a sequence of Map types.
 Every Map type associates a const char* to a type.
 The user defined class must implement 2 methods:
 1. virtual void set(const char* const key, void* const item)
       item is guaranteed to be of the type corresponding to key in the Map,
       the method must set the corresponding field of this appropriately.
 2. virtual const void* get(const char* const key) const
       these method must return a pointer to the corresponding field of this.

 See example.cpp for a working code example.
*/

#ifndef __COMMON_JSON_HPP__
#define __COMMON_JSON_HPP__

#include "json.hpp" // from: https://github.com/nlohmann/json

namespace Json{
  using Key_t = const char* const;
  /*!
    \brief class used to define single mapping: char* -> type

    Use these class to define json key, value type mapping
    when inheriting from Json
  */
  template <Key_t _key, typename _T>
  struct Map{
    static constexpr Key_t key = _key;
    typedef _T Type;
  };

  /*!
    \brief inner class that implements (de)serialization

    User of the library should never use these class,
    use Json class instead.
    Implements the base case for Types variadic template.
  */
  template <typename ...Types>
  class __Json__{
  protected:
    /*!
      \brief convert this to json: serialize.
     */
    constexpr void toJson(nlohmann::json& j) const{}

    /*!
      \brief convert json to this: deserialize.
     */
    constexpr void fromJson(const nlohmann::json& j){}

    /*!
      \brief virtual method: set object identified by key to item.
     */
    virtual void set(Key_t key, void* const item) = 0;

    /*!
      \brief virtual method: return object identified by key.
     */
    virtual const void* get(Key_t key) const = 0;
  };


  /*!
    \brief inner class that implements (de)serialization

    User of the library should never use these class,
    use Json class instead.
    Implements the set for Types variadic template.
  */
  template < typename T, typename ...Types >
  class __Json__<T, Types...> : protected __Json__<Types ...>
  {
  protected:
    /*!
      \brief convert this to json: serialize.
     */
    constexpr void toJson(nlohmann::json& j) const;

    /*!
      \brief convert json to this: deserialize.
     */
    constexpr void fromJson(const nlohmann::json& j);

    /*
      virtual get and set inherited from base case.
     */
  };

  template <typename T, typename ...Types >
  constexpr void __Json__<T, Types...>::toJson(nlohmann::json& j) const{
    j[T::key] = *(static_cast<const typename T::Type *>(this->get(T::key)));
    __Json__<Types...>::toJson(j);
  }


  template <typename T, typename ...Types >
  constexpr void __Json__<T, Types...>::fromJson(const nlohmann::json& j){
    typename T::Type alias = j.at((Key_t)T::key).get<typename T::Type>();
    this->set(T::key, &alias);
    __Json__<Types...>::fromJson(j);
  }


  /*!
    \brief interface to user

    These class hiddens the inheritance chain of __Json__
    making it possible to call to_json and from_json without ambiguity,
    these 2 methods allow to serialize and deserialize subclasses of Json
    using assignment to json objects (nlohmann::json interface).
   */
  template <typename ...Ts >
  class Json : private __Json__<Ts...>{
  public:
    /*!
      \brief serialize this into j.
     */
    constexpr void toJson(nlohmann::json& j) const{
      __Json__<Ts...>::toJson(j);
    }
    /*!
      \brief build this from j (deserialization).
     */
    constexpr void fromJson(const nlohmann::json& j){
      __Json__<Ts...>::fromJson(j);
    }
  };


  /*!
    \brief extend nlohmann serialization interface to work with Json objects.
  */
  template <typename ... Ts>
  constexpr void to_json(nlohmann::json& j, const Json<Ts...>& obj){
    obj.toJson(j);
  }

  /*!
    \brief extend nlohmann deserialization interface to work with Json objects.
  */
  template <typename ... Ts>
  constexpr void from_json(const nlohmann::json& j, Json<Ts...>& obj){
    obj.fromJson(j);
  }

}

#endif /* __COMMON_JSON_HPP__ */
