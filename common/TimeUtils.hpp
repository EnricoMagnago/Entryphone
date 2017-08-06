/*!****************************************************************************
 *    \file    TimeUtils.hpp
 *    \brief   A set of template definitions for time and periodic entities.
 *    \version 1.1
 *    \date    2017
 *****************************************************************************/
/// @file   TimeUtils.hpp
/// @brief  A set of template definitions for periodic entities.
/// @author Valerio Magnago          <valerio.magnago@unitn.it>
///         Paolo Bevilacqua         <paolo.bevilacqua@unitn.it>
///
/// \version 1.0
/// @author Luigi Palopoli           <luigi.palopoli@unitn.it>
///         Bernardo Villalba Frías  <br.villalbafrias@unitn.it>
///         Alessio Colombo          <alessio.colombo_1@unitn.it>




#ifndef __TIME_UTILS_HPP__
#define __TIME_UTILS_HPP__

//  -------- Standard lib include -------------
#include <ratio>
#include <time.h>

namespace TimeUtils {

    /// Assert for wrong
    template <typename T, template <intmax_t...> typename Template>
    struct is_ratio_specialization: std::false_type {};

    template <template <intmax_t...> typename Template, intmax_t... Args>
    struct is_ratio_specialization<Template<Args...>, Template> : std::true_type {};




    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                           GENERAL TIME TEMPLATE                                                         //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Internal structure for GeneralTime_
    template<typename value_type, typename internal_sec_ratio = std::milli>
    struct GeneralTime_ {

        static_assert(is_ratio_specialization<internal_sec_ratio, std::ratio>{}, "Second element of template list must be s std::ratio type");
        static_assert(std::is_integral<value_type>::value,"First template type must be an integer");

        /// To give access to others timeUtils template private members
        //friend class GeneralTime_;

    public:
        value_type value;  // struct member (time)


        /// @brief convert duration from input_sec_ratio to output_sec_ratio
        template<typename input_sec_ratio, typename output_sec_ratio, typename value_type_output, typename value_type_input>
        static value_type_output convert_value(value_type_input duration_) {
            // Static asserts
            static_assert(is_ratio_specialization<input_sec_ratio, std::ratio>{},  "input_sec_ratio must be a std::ratio type");
            static_assert(is_ratio_specialization<output_sec_ratio, std::ratio>{}, "output_sec_ratio must be a std::ratio type");
            static_assert(std::is_integral<value_type_input>::value,"value_type_input must be an integer");
            static_assert(std::is_integral<value_type_output>::value,"value_type_output must be an integer");
            // input_sec_ratio  is the ratio of the input value
            // output_sec_ratio is the ratio of the output value
            // value is the value type (integer value)
            return (((value_type_output)duration_) * output_sec_ratio::den * input_sec_ratio::num) / (input_sec_ratio::den * output_sec_ratio::num );
        }

    public:

        /// @brief default constructor
        GeneralTime_() : value(0){ }

        /// @brief initializator
        GeneralTime_(value_type v) : value(v) {};

        /// @brief copy constructor
        template<typename value_type_tmp, typename input_sec_ratio>
        GeneralTime_(GeneralTime_<value_type_tmp,input_sec_ratio> gt_) {
            value = gt_.get_duration<internal_sec_ratio,value_type>();
        }

        /// @brief return get the duration value in the desired ratio (i.e. output_sec_ratio)
        template<typename output_sec_ratio,typename value_type_output>
        inline value_type_output get_duration() const {
            // Convert from internal_sec_ratio to output_sec_ratio
            return convert_value<internal_sec_ratio,output_sec_ratio,value_type_output>(value);
        }


        /// @brief  add a specific duration to the current value
        template<typename input_ratio, typename input_value>
        void increment_duration(input_value val) {
            static_assert(std::is_integral<input_value>::value,"input_value must be an integer");
            value = value + convert_value<input_ratio,internal_sec_ratio,value_type>(val);
        }


        /// @brief return set the duration value from a desired ratio (i.e. input_sec_ratio)
        template<typename input_sec_ratio>
        void set_duration(value_type duration_) {
            // Convert from input_sec_ratio to internal sec ratio
            this->value = convert_value<input_sec_ratio,internal_sec_ratio,value_type>(duration_);
            return;
        }

        // Get fraction value
        static intmax_t get_num()  {return  internal_sec_ratio::num;}
        static intmax_t get_den()  {return  internal_sec_ratio::den;}
        static double   get_ratio(){return  get_num()/(double)get_den();}

        // SPECIALIZE SOME GETTER
        /// @brief specialize getter for milliseconds
        template<typename value_type_output>
        value_type get_msec() const { return get_duration<std::milli,value_type_output>();}
        /// @brief specialize getter for microseconds
        template<typename value_type_output>
        value_type get_usec() const { return get_duration<std::micro,value_type_output>();}
        /// @brief specialize getter for nanoseconds
        template<typename value_type_output>
        value_type get_nsec() const { return get_duration<std::nano,value_type_output>(); }

        // SPECIALIZE SOME SETTER
        /// @brief specialize setter for milliseconds
        void set_msec(value_type duration_) { return set_duration<std::milli>(duration_);}
        /// @brief specialize setter for microseconds
        void set_usec(value_type duration_) { return set_duration<std::micro>(duration_);}
        /// @brief specialize setter for nanoseconds
        void set_nsec(value_type duration_) { return set_duration<std::nano>(duration_); }



        /// @brief Define assigment between GeneralTime_ with different granularity
        template<typename value_type_input, typename input_sec_ratio>
        GeneralTime_<value_type, internal_sec_ratio> &operator=(const GeneralTime_<value_type_input,input_sec_ratio> &c) {

            // Assert if bad input
            static_assert(is_ratio_specialization<input_sec_ratio, std::ratio>{}, "Second element of template list must be s std::ratio type");
            static_assert(std::is_integral<value_type_input>::value,"First template type must be an integer");

            // Convert value properly
            value = c.get_duration<internal_sec_ratio,value_type>();

            return *this;
        }

        ///< OVERLOAD BINARY ARITHMETIC OPERATORS
        /// @brief overload operator<
        template<typename sec_ratio_l, typename value_type_l, typename sec_ratio_r, typename value_type_r>
        friend inline bool operator< (const GeneralTime_<value_type_l,sec_ratio_l>& t_l, const GeneralTime_<value_type_r,sec_ratio_r>& t_r);
        /// >, <=, >= are deduced from this one


        /// @brief overload operator==
        template<typename sec_ratio_l, typename value_type_l, typename sec_ratio_r, typename value_type_r>
        friend inline bool operator== (const GeneralTime_<value_type_l,sec_ratio_l>& t_l, const GeneralTime_<value_type_r,sec_ratio_r>& t_r);

        /// @brief overload operator a+b
        template<typename sec_ratio_l, typename value_type_l, typename sec_ratio_r, typename value_type_r>
        friend inline GeneralTime_<value_type_l,sec_ratio_l> operator+ (const GeneralTime_<value_type_l,sec_ratio_l>& t_l, const GeneralTime_<value_type_r,sec_ratio_r>& t_r);


        /// @brief overload operator a-b
        template<typename sec_ratio_l, typename value_type_l, typename sec_ratio_r, typename value_type_r>
        friend inline GeneralTime_<value_type_l,sec_ratio_l> operator- (const GeneralTime_<value_type_l,sec_ratio_l>& t_l, const GeneralTime_<value_type_r,sec_ratio_r>& t_r);


        /// @brief overload operator *
        template<typename sec_ratio_l, typename value_type_l, typename NumericType>
        friend inline GeneralTime_<value_type_l,sec_ratio_l> operator*(const GeneralTime_<value_type_l,sec_ratio_l>& t_l, const NumericType v);

        /// @brief get clock time and put it in the GeneralTime structure
        void _getSystemTime() {
            timespec time;
            //CLOCK_MONOTONIC
            clock_gettime(CLOCK_REALTIME, &time);
            this->set_duration<std::ratio<1,1>>(time.tv_sec);
            this->increment_duration<std::nano>(time.tv_nsec);
            return;
        }

    };


    ///< OVERLOAD ARITHMETIC OPERATORS
    /// @brief overload operator +
    template<typename sec_ratio_l, typename value_type_l, typename sec_ratio_r, typename value_type_r>
    inline GeneralTime_<value_type_l,sec_ratio_l> operator+ (const GeneralTime_<value_type_l,sec_ratio_l>& t_l, const GeneralTime_<value_type_r,sec_ratio_r>& t_r){
        //value_type_l value = ;
        //t_l.get_duration<std::nano>();

        value_type_l value = t_l.template get_duration<sec_ratio_l,value_type_l>() + t_r.template get_duration<sec_ratio_l,value_type_l>();//t_l.get_duration<std::milli>();
        GeneralTime_<value_type_l,sec_ratio_l> output(value);
        //GeneralTime_<value_type_l,sec_ratio_l> output(t_l.get_duration<sec_ratio_l>() + t_r.get_duration<sec_ratio_l>());   // convert b in a fraction and sum
        return output;
    }


    /// @brief overload operator -
    template<typename sec_ratio_l, typename value_type_l, typename sec_ratio_r, typename value_type_r>
    inline GeneralTime_<value_type_l,sec_ratio_l> operator- (const GeneralTime_<value_type_l,sec_ratio_l>& t_l, const GeneralTime_<value_type_r,sec_ratio_r>& t_r){
        //value_type_l value = ;
        //t_l.get_duration<std::nano>();

        value_type_l value = t_l.template get_duration<sec_ratio_l,value_type_l>() - t_r.template get_duration<sec_ratio_l,value_type_l>();//t_l.get_duration<std::milli>();
        GeneralTime_<value_type_l,sec_ratio_l> output(value);
        //GeneralTime_<value_type_l,sec_ratio_l> output(t_l.get_duration<sec_ratio_l>() + t_r.get_duration<sec_ratio_l>());   // convert b in a fraction and sum
        return output;
    }

    /// @brief overload operator *
    template<typename sec_ratio_l, typename value_type_l, typename NumericType>
    GeneralTime_<value_type_l,sec_ratio_l> operator*(const GeneralTime_<value_type_l,sec_ratio_l>& t_l, const NumericType v) {
        static_assert(std::is_integral<NumericType>::value,"First template type must be an integer");
        GeneralTime_<value_type_l,sec_ratio_l> output(v* (t_l.template get_duration<sec_ratio_l,value_type_l>()));
        return output;
    }

    ///< OVERLOAD BINARY ARITHMETIC OPERATORS
    /// @brief overload operator<
    template<typename sec_ratio_l, typename value_type_l, typename sec_ratio_r, typename value_type_r>
    inline bool operator< (const GeneralTime_<value_type_l,sec_ratio_l>& t_l, const GeneralTime_<value_type_r,sec_ratio_r>& t_r){
        return (t_l.value * sec_ratio_l::num * sec_ratio_r::den) <  (t_r.value * sec_ratio_r::num * sec_ratio_l::den);
    }

    /// @brief overload operator==
    template<typename sec_ratio_l, typename value_type_l, typename sec_ratio_r, typename value_type_r>
    inline bool operator== (const GeneralTime_<value_type_l,sec_ratio_l>& t_l, const GeneralTime_<value_type_r,sec_ratio_r>& t_r) {
        return (t_l.value * sec_ratio_l::num * sec_ratio_r::den) == (t_r.value * sec_ratio_r::num * sec_ratio_l::den);
    }

    /// >, <=, >= are deduced from this one
    template<typename sec_ratio_l, typename value_type_l, typename sec_ratio_r, typename value_type_r>
    inline bool operator> (const GeneralTime_<value_type_l,sec_ratio_l>& t_l, const GeneralTime_<value_type_r,sec_ratio_r>& t_r){ return t_r < t_l; }
    template<typename sec_ratio_l, typename value_type_l, typename sec_ratio_r, typename value_type_r>
    inline bool operator<=(const GeneralTime_<value_type_l,sec_ratio_l>& t_l, const GeneralTime_<value_type_r,sec_ratio_r>& t_r){ return !(t_l > t_r); }
    template<typename sec_ratio_l, typename value_type_l, typename sec_ratio_r, typename value_type_r>
    inline bool operator>=(const GeneralTime_<value_type_l,sec_ratio_l>& t_l, const GeneralTime_<value_type_r,sec_ratio_r>& t_r){ return !(t_l < t_r); }
    template<typename sec_ratio_l, typename value_type_l, typename sec_ratio_r, typename value_type_r>
    inline bool operator!=(const GeneralTime_<value_type_l,sec_ratio_l>& t_l, const GeneralTime_<value_type_r,sec_ratio_r>& t_r){ return !(t_l == t_r); }
//  __________________________________________________________________________________________________________________________________________________ //







    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                  GENERAL TIME CONSTANT TEMPLATE                                                         //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /// @brief general way to express a time constant
    ///
    /// A time constant is actually a type that can be used for compile time consistency
    /// checks (e.g., to enforce that a port producing data at 10ms is connected to a
    /// sensor receiving data at the same period). The constant is a template of a numeric
    /// type (used for internal representation), a value and a graularity.

    /// Internal representation used for constant GeneralTime_
    template<typename numeric_type, numeric_type V, typename internal_sec_ratio = std::milli>
    struct GeneralTimeConstant_ : public std::integral_constant<numeric_type, V> {
        static_assert(is_ratio_specialization<internal_sec_ratio, std::ratio>{}, "Second element of template list must be s std::ratio type");
        static_assert(std::is_integral<numeric_type>::value,"First template type must be an integer");

    private:
        // Define static value
        using typename std::integral_constant<numeric_type, V>::value_type;
        using std::integral_constant<numeric_type, V>::value;


    public:
        // Get fraction value
        static intmax_t get_num()  {return  internal_sec_ratio::num;}
        static intmax_t get_den()  {return  internal_sec_ratio::den;}
        static double   get_ratio(){return  get_num()/(double)get_den();}



        /// @brief return the value in the desired ratio of second (i.e.
        /// get_duration<std::milli>() return the duration expessed in milliseconds)
        ///
        template<typename output_sec_ratio>
        static numeric_type get_duration() {
            static_assert(is_ratio_specialization<output_sec_ratio, std::ratio>{}, "get_duration want a std::ratio type");
            return value * output_sec_ratio::den * internal_sec_ratio::num / (internal_sec_ratio::den * output_sec_ratio::num );
        }

        // get time with a particular granularity
        static numeric_type get_msec() { return get_duration<std::milli>();}
        static numeric_type get_usec() { return get_duration<std::micro>();}
        static numeric_type get_nsec() { return get_duration<std::nano>(); }


        static constexpr bool is_null = (value == 0);
        static constexpr bool is_not_null = (value != 0);


    };

    // TODO: overload same operator as general time, implementing cross operation

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                           TIMERS                                                                        //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ///@brief Instrumental class for timers
    ///
    /// This particular timer uses the POSIX's clock nanosleep
    /// facility. This class is a template of integer type and
    /// tick_per_sec.
    template<typename NumericType, typename internal_sec_ratio>
    class PeriodicTimer_t {
        struct timespec r;
        GeneralTime_<NumericType,internal_sec_ratio> period;

    protected:
        ///@brief increments the interal spec by a specified amount
        ///
        ///@param d quantity to sum up to the internal timer. It is specified in ticks.
        ///       Therefore the summed amount is d/ticks_per_sec
        template<typename NumericType_aux, typename sec_ratio_aux>
        void add(const GeneralTime_<NumericType_aux,sec_ratio_aux> &d) {
            uint64_t tmp = d.template get_duration<std::nano,uint64_t>();
            tmp += r.tv_nsec;                      // add to tmp the nanoseconds in the timer clock
            while (tmp >= 1000000000ULL) {
                tmp -= 1000000000ULL;
                r.tv_sec += 1;                    // remove 1 second to the nanosecond and increment second counter
            }
            r.tv_nsec = tmp;                      // now that nanosecond are less than the nanosecon in a second assign to r.tv_nsec the time in nanosecond
        }

    public:

        /// @brief Constructor for the class
        ///
        /// @param per Period specified in ticks.
        template<typename input_numeric>
        PeriodicTimer_t(input_numeric value){
            GeneralTime_<input_numeric,internal_sec_ratio> per(value);
            setPeriod(per);
        }


        /// @brief Constructor for the class
        ///
        /// @param per Period specified in ticks.
        template<typename input_numeric, typename input_ratio>
        PeriodicTimer_t(GeneralTime_<input_numeric,input_ratio> per){
            setPeriod(per);
        }


        /// @brief Set the period of the timer
        ///
        /// @param set period of the timer
        template<typename NumericType_aux, typename sec_ratio_aux>
        void setPeriod(GeneralTime_<NumericType_aux,sec_ratio_aux> per) {
            period = per;
        }

        /// @brief get the period of the timer
        ///
        /// @param get period of the timer
        GeneralTime_<NumericType,internal_sec_ratio> getPeriod() {
            return period;
        }

        /// @brief Starts the timer
        ///
        /// @param offset Intial offset before the first time the timer expires
        template<typename NumericType_aux, typename sec_ratio_aux>
        void start(const GeneralTime_<NumericType_aux,sec_ratio_aux> offset) {
            clock_gettime(CLOCK_MONOTONIC, &r);
            add(offset);
        }

        /// @brief Starts the timer
        ///
        void start() {
            clock_gettime(CLOCK_MONOTONIC, &r);
        }

        /// @brief Blocks the caller untile the next timer expiration.
        /// Each periodic task should call this as a first operation and then
        /// be blocked untile the next activation firing.
        void wait_for_next_activation() {
            clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &r, NULL);

            // std::cout << "Tempo attuale :    " << r.tv_sec <<"[s]\t\t" << r.tv_nsec <<  "[ns]"<<std::endl;
            add(period);
            // std::cout << "Tempo fine ciclo:  " <<  r.tv_sec <<"[s]\t\t" << r.tv_nsec << "[ns]"<<std::endl;
            // std::cout << "Differenza:        \t\t\t" <<  period*1000000000ULL/1000 <<"[ns]"<<std::endl;
        }
    };
}
#endif // __TIME_UTILS_HPP__
