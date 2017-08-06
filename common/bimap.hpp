/*
 * bimap.hpp
 *
 *  Created on: Jun 30, 2015
 *      Author: "Alessio Colombo <colombo@disi.unitn.it>"
 */
#ifndef COMMON_BIMAP_HPP_
#define COMMON_BIMAP_HPP_

#include <vector>
#include <map>
#include <initializer_list>


template <typename Tleft, typename Tright>
class bimap {
  private:
    typedef std::map<Tleft,Tright> Tleft_to_Tright_t;
    typedef std::map<Tright,Tleft> Tright_to_Tleft_t;

    Tleft_to_Tright_t Tleft_to_Tright;
    Tright_to_Tleft_t Tright_to_Tleft;

  public:

    typedef std::pair<Tleft,Tright> pair_t;

    bimap() {}

    // initializer_list input
    bimap(const std::initializer_list<pair_t>& in) {
      for (const pair_t& p : in) {
        Tleft_to_Tright[p.first] = p.second;
        Tright_to_Tleft[p.second] = p.first;
      }
    }

    const Tleft_to_Tright_t& l2r() const {return Tleft_to_Tright;}
    const Tright_to_Tleft_t& r2l() const {return Tright_to_Tleft;}

    // Tlefthe function throws an out_of_range exception if index in not in the map
    const Tleft& operator[](const Tright& index) const {
      return Tright_to_Tleft.at(index);
    }

    // Tlefthe function throws an out_of_range exception if index in not in the map
    const Tright& operator[](const Tleft& index) const {
      return Tleft_to_Tright.at(index);
    }

    virtual ~bimap() {};


};



#endif /* COMMON_BIMAP_HPP_ */
