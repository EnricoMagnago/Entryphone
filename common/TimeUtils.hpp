/*!****************************************************************************
 *    \file    TimeUtils.hpp
 *    \brief   A set of template definitions for periodic entities.
 *    \version 1.0
 *    \date    2015
 *****************************************************************************/
/// @file   TimeUtils.hpp
/// @brief  A set of template definitions for periodic entities.
/// @author Luigi Palopoli           <luigi.palopoli@unitn.it>
///         Bernardo Villalba Frías  <br.villalbafrias@unitn.it>
///         Alessio Colombo          <alessio.colombo_1@unitn.it>


#ifndef TIME_UTILS_HPP
#define TIME_UTILS_HPP

#include <chrono>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <mutex>

namespace AcantoCommon {

  using standard_usec =  std::chrono::microseconds;
  using standard_msec =  std::chrono::milliseconds;
  using standard_nsec =  std::chrono::nanoseconds;

  ///@brief enum class to be used to specify the granularity of time representations
  enum class Granularity {
    MILLI, MICRO, NANO
  };

    /// Internal structure for AcantoTime
  template<typename NumericType, int ticks_per_sec=1000>
  struct AcantoTime_ {
    typedef NumericType value_type;
    value_type value;

    NumericType get_usec() const {  return value * 1000000/ticks_per_sec; }
    NumericType get_msec() const {  return value * 1000/ticks_per_sec; }
    NumericType get_nsec() const {  return value * 1000000000/ticks_per_sec; }

    standard_usec  get_standard_usec() const {return standard_usec(value * 1000000/ticks_per_sec);};
    standard_msec  get_standard_msec() const  {return standard_msec(value * 1000/ticks_per_sec);};
    standard_nsec  get_standard_nsec()  const {return standard_nsec(value * 1000000000/ticks_per_sec);};

    AcantoTime_(NumericType v) :value(v) {};
    AcantoTime_(const AcantoTime_<NumericType, ticks_per_sec> & c) {
      operator=(c);
    };
    AcantoTime_<NumericType, ticks_per_sec> & operator=(const AcantoTime_<NumericType, ticks_per_sec> & c) {
      value = c.value;
      return *this;
    }

  };


  template <typename NumericType, Granularity g>
  struct AcantoTime {};

  template <typename NumericType>
  struct AcantoTime<NumericType, Granularity::MILLI>: public AcantoTime_<NumericType, 1000> {
    AcantoTime(NumericType v=0) : AcantoTime_<NumericType,1000>(v) {};

    AcantoTime(const standard_msec & v): AcantoTime_<NumericType,1000>(v.count()) {};
    AcantoTime(const AcantoTime<NumericType,Granularity::MILLI> & c):AcantoTime_<NumericType, 1000>(c) {
    };
    AcantoTime<NumericType,Granularity::MILLI> & operator=(const AcantoTime<NumericType,Granularity::MILLI> & c) {
      AcantoTime_<NumericType, 1000>::operator=(c);
      return *this;
    }

    //void setToNow() { value = getSystemTime<NumericType, 1000>(); };
  };

  template <typename NumericType>
  struct AcantoTime<NumericType, Granularity::MICRO>: public AcantoTime_<NumericType,1000000> {
    AcantoTime(NumericType v=0) : AcantoTime_<NumericType,1000000>(v) {};
    AcantoTime(const standard_usec & v): AcantoTime_<NumericType,1000000>(v.count()) {};
    AcantoTime(const AcantoTime<NumericType,Granularity::MICRO> & c):AcantoTime_<NumericType, 1000000>(c) {
    };
    AcantoTime<NumericType,Granularity::MICRO> & operator=(const AcantoTime<NumericType,Granularity::MICRO> & c) {
      AcantoTime_<NumericType, 1000000>::operator=(c);
      return *this;
    }
    //void setToNow() { value = getSystemTime<NumericType, 1000000>(); };
  };

  template <typename NumericType>
  struct AcantoTime<NumericType, Granularity::NANO>: public AcantoTime_<NumericType, 1000000000> {
    AcantoTime(NumericType v=0) : AcantoTime_<NumericType,1000000000>(v) {};
    AcantoTime(const standard_nsec & v): AcantoTime_<NumericType,1000000000>(v.count()) {};
    AcantoTime(const AcantoTime<NumericType,Granularity::NANO> & c):AcantoTime_<NumericType, 1000000000>(c)  {
      operator=(c);
    };
    AcantoTime<NumericType,Granularity::NANO> & operator=(const AcantoTime<NumericType,Granularity::NANO> & c) {
      AcantoTime_<NumericType, 1000000000>::operator=(c);
      return *this;
    }
    //void setToNow() { value = getSystemTime<NumericType, 1000000000>(); };
  };




  /// Internal representation used for AcantoTimeConstants
  template<typename NumericType, NumericType V, int ticks_per_sec=1000>
  struct AcantoTimeConstant_: public std::integral_constant<NumericType, V> {
    using typename std::integral_constant<NumericType, V>::value_type;
    using std::integral_constant<NumericType, V>::value;


    static NumericType get_usec() {  return value * 1000000/ticks_per_sec; }
    static NumericType get_msec() {  return value * 1000/ticks_per_sec; }
    static NumericType get_nsec() {  return value * 1000000000/ticks_per_sec; }


    static standard_usec  get_standard_usec()  {return standard_usec(value * 1000000/ticks_per_sec);};
    static standard_msec  get_standard_msec()  {return standard_msec(value * 1000/ticks_per_sec);};
    static standard_nsec  get_standard_nsec()  {return standard_nsec(value * 1000000000/ticks_per_sec);};

    static constexpr bool is_null = (value == 0);
    static constexpr bool is_not_null = (value != 0);

  };

  /// @brief general way to express a time constant in Acanto
  ///
  /// A time constant is actually a type that can be used for compile time consistency
  /// checks (e.g., to enforce that a port producing data at 10ms is connected to a
  /// sensor receiving data at the same period). The constant is a template of a numeric
  /// type (used for internal representation), a value and a graularity.
  template <typename NumericType,  NumericType V, Granularity g>
  struct AcantoTimeConstant {
  };

  template <typename NumericType, NumericType V>
  struct AcantoTimeConstant<NumericType, V, Granularity::MILLI>: public AcantoTimeConstant_<NumericType, V, 1000> {
    static AcantoTime<NumericType, Granularity::MILLI> getAcantoMs() {return AcantoTime<NumericType, Granularity::MILLI>(V);};
    static AcantoTime<NumericType, Granularity::MICRO> getAcantoUs() {return AcantoTime<NumericType, Granularity::MICRO>(V*1000);};
    static AcantoTime<NumericType, Granularity::NANO> getAcantoNs() {return AcantoTime<NumericType, Granularity::NANO>(V*1000*1000);};
  };

  template <typename NumericType, NumericType V>
  struct AcantoTimeConstant<NumericType, V, Granularity::MICRO>: public AcantoTimeConstant_<NumericType,V, 1000000> {
    static AcantoTime<NumericType, Granularity::MICRO> getAcantoTimeUs() {return AcantoTime<NumericType, Granularity::MICRO>(V);};
    static AcantoTime<NumericType, Granularity::NANO>  getAcantoTimeNs() {return AcantoTime<NumericType, Granularity::MICRO>(V*1000);};
  };


  template <typename NumericType, NumericType V>
  struct AcantoTimeConstant<NumericType, V, Granularity::NANO>: public AcantoTimeConstant_<NumericType,V, 1000000000> {
    static AcantoTime<NumericType, Granularity::NANO> getAcantoTimeNs() {return AcantoTime<NumericType, Granularity::NANO>(V);};
  };

  //AcantoTime vs AcantoTimeConstant operators

  template<typename NumericType,  NumericType V, Granularity g>
  AcantoTime<NumericType, g>  operator+(AcantoTime<NumericType, g> & a1, AcantoTimeConstant<NumericType, V, g>& a2) {return AcantoTime<NumericType, g>(a1.value+a2.value);}

  template<typename NumericType,  NumericType V, Granularity g>
  AcantoTime<NumericType, g>  operator+( AcantoTimeConstant<NumericType, V, g>& a2, AcantoTime<NumericType, g> & a1 ) {return AcantoTime<NumericType, g>(a1.value+a2.value);}


  //AcantoTime vs AcantoTime operators
  template<typename NumericType,  Granularity g>
  AcantoTime<NumericType, g>  operator+(const AcantoTime<NumericType, g> & a1, const AcantoTime<NumericType, g>& a2) {return AcantoTime<NumericType, g>(a1.value+a2.value);}

  template<typename NumericType,  Granularity g>
  AcantoTime<NumericType, g>  operator-(const AcantoTime<NumericType, g> & a1, const AcantoTime<NumericType, g>& a2) {return AcantoTime<NumericType, g>(a1.value-a2.value);}

  template<typename NumericType,  Granularity g>
  NumericType  operator/(const AcantoTime<NumericType, g> & a1, const AcantoTime<NumericType, g>& a2) {return a1.value/a2.value;}



  template<typename NumericType, Granularity g>
  bool  operator<(const AcantoTime<NumericType, g> & a1, const AcantoTime<NumericType, g>& a2) { return a1.value < a2.value; }

  template<typename NumericType, Granularity g>
  bool  operator>(const AcantoTime<NumericType, g> & a1, const AcantoTime<NumericType, g>& a2) { return a1.value > a2.value; }

  template<typename NumericType, Granularity g>
  bool  operator!=(const AcantoTime<NumericType, g> & a1, const AcantoTime<NumericType, g>& a2) { return a1.value != a2.value; }

  template<typename NumericType, Granularity g>
  bool  operator==(const AcantoTime<NumericType, g> & a1, const AcantoTime<NumericType, g>& a2) { return a1.value == a2.value; }


  //AcantoTime vs NumericType
  template<typename NumericType, Granularity g>
  AcantoTime<NumericType, g>  operator*(NumericType v, const AcantoTime<NumericType, g>& a2) {return AcantoTime<NumericType, g>(v*a2.value);}

  template<typename NumericType,  Granularity g>
  AcantoTime<NumericType, g>  operator*(const AcantoTime<NumericType, g>& a2, NumericType v) {return AcantoTime<NumericType, g>(v*a2.value);}


  /// Utility functions
  template<typename NumericType, int ticks_per_sec>
  NumericType _getSystemTime() {
    timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);

    return (1000000000ULL*time.tv_sec+time.tv_nsec)/(1000000000ULL/ticks_per_sec);
  }


  template <typename NumericType, Granularity g>
  void getSystemTime(AcantoTime<NumericType, g> & t) {}

  template <typename NumericType>
  void getSystemTime(AcantoTime<NumericType, Granularity::MILLI> & t) {
    t =AcantoTime<NumericType, Granularity::MILLI>( _getSystemTime<NumericType,1000ULL>());
  }

  template <typename NumericType>
  void getSystemTime(AcantoTime<NumericType, Granularity::MICRO> & t) {
    t =AcantoTime<NumericType, Granularity::MICRO>( _getSystemTime<NumericType,1000000ULL>());
  }

  template <typename NumericType>
  void getSystemTime(AcantoTime<NumericType, Granularity::NANO> & t) {
    t =AcantoTime<NumericType, Granularity::NANO>( _getSystemTime<NumericType,1000000000ULL>());
  }







  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ///                                                           TIMERS                                                                        //
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  ///@brief Instrumental class for timers
  ///
  /// This particular timer uses the POSIX's clock nanosleep
  /// facility. This class is a template of integer type and
  /// tick_per_sec.
  template <typename INT_TYPE, int tick_per_sec>
  class PeriodicTimer_ {
    struct timespec r;
    INT_TYPE period;
    std::mutex mtx;
  protected:
    ///@brief increments the interal spec by a specified amount
    ///
    ///@param d quantity to sum up to the internal timer. It is specified in ticks.
    ///       Therefore the summed amount is d/ticks_per_sec
    void add(INT_TYPE d) {
      d *= 1000000000ULL / tick_per_sec;
      d += r.tv_nsec;
      while (d >= 1000000000ULL) {
        d -= 1000000000ULL;
        r.tv_sec += 1;
      }
      r.tv_nsec = d;
    };

  public:
    /// @brief Constructor for the class
    ///
    /// @param per Period specified in ticks.
    PeriodicTimer_(INT_TYPE per): period(per) {};

    /// @brief Starts the timer
    ///
    /// @param offset Intial offset before the first time the timer expires
    void start(INT_TYPE offset) {
      clock_gettime(CLOCK_MONOTONIC, &r);
      add(offset);
    }

    /// @brief Blocks the caller untile the next timer expiration.
    /// Each periodic task should call this as a first operation and then
    /// be blocked untile the next activation firing.
    void wait_for_next_activation() {
      clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &r, NULL);
      std::unique_lock<std::mutex> mtx_loc(mtx);
      add(period);
    }

     /// @brief This block set the activation period.
     void setPeriod(const INT_TYPE per){
       std::unique_lock<std::mutex> mtx_loc(mtx);
       period = per;
     }


  };


  template <typename INT_TYPE, Granularity g>
  class PeriodicTimer {};

  ///@brief Periodic timer spcialised for milli seconds.
  ///
  ///The period has to be specified in milliseconds.
  template <typename INT_TYPE>
  class PeriodicTimer<INT_TYPE, Granularity::MILLI>: public PeriodicTimer_<INT_TYPE,1000> {
  public:
    PeriodicTimer(INT_TYPE per): PeriodicTimer_<INT_TYPE,1000>(per) {};
  };

  ///@brief Periodic timer spcialised for micro seconds.
  ///
  ///The period has to be specified in microseconds.
  template <typename INT_TYPE>
  class PeriodicTimer<INT_TYPE, Granularity::MICRO>: public PeriodicTimer_<INT_TYPE,1000000> {
  public:
    PeriodicTimer(INT_TYPE per): PeriodicTimer_<INT_TYPE,1000000>(per) {};
  };

  ///@brief Periodic timer spcialised for nanoseconds.
  ///
  ///The period has to be specified in nanoseconds.
  template <typename INT_TYPE>
  class PeriodicTimer<INT_TYPE, Granularity::NANO>: public PeriodicTimer_<INT_TYPE,1000000000> {
  public:
    PeriodicTimer(INT_TYPE per): PeriodicTimer_<INT_TYPE,1000000000>(per) {};
  };

  typedef PeriodicTimer<uint64_t, Granularity::MILLI> PeriodicTimerMs;
}
#endif
