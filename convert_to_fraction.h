
#pragma once

#include <boost/rational.hpp>
#include <limits>
#include <numbers>
#include <exception>
#include <format>
#include <iostream>
#include <cassert>

namespace cvt_2_fraction
{
	constexpr bool DebugReporting = true;

	template<typename int_type>
	using Fraction = boost::rational::rational<int_type>;

	template<typename int_type>
	double toFloat(const Fraction<int_type> &frac) {
			return double(frac.numerator()) / double(frac.denominator());
		}

	template<typename int_type>
	std::string toString(const Fraction<int_type> &frac) {
			return std::format("{}/{}", frac.numerator(), frac.denominator());
		}



    /// <summary>
    /// <para>Code according to info found here: http://mathforum.org/library/drmath/view/51886.html</para>
    /// 
    /// <para>
    /// Date: 06/29/98 at 13:12:44</para>
    /// <para>
    /// From: Doctor Peterson</para>
    /// <para>
    /// Subject: Re: Decimal To Fraction Conversion</para>
    /// 
    /// <para>
    /// The algorithm I am about to show you has an interesting history. I 
    /// recently had a discussion with a teacher in England who had a 
    /// challenging problem he had given his students, and wanted to know what 
    /// others would do to solve it. The problem was to find the fraction 
    /// whose decimal value he gave them, which is essentially identical to 
    /// your problem! I wasn't familiar with a standard way to do it, but 
    /// solved it by a vaguely remembered Diophantine method. Then, my 
    /// curiosity piqued, and I searched the Web for information on the 
    /// problem and didn't find it mentioned in terms of finding the fraction 
    /// for an actual decimal, but as a way to approximate an irrational by a 
    /// fraction, where the continued fraction method was used. </para>
    /// 
    /// <para>
    /// I wrote to the teacher, and he responded with a method a student of 
    /// his had come up with, which uses what amounts to a binary search 
    /// technique. I recognized that this produced the same sequence of 
    /// approximations that continued fractions gave, and was able to 
    /// determine that it is really equivalent, and that it is known to some 
    /// mathematicians (or at least math historians). </para>
    /// 
    /// <para>
    /// After your request made me realize that this other method would be 
    /// easier to program, I thought of an addition to make it more efficient, 
    /// which to my knowledge is entirely new. So we're either on the cutting 
    /// edge of computer technology or reinventing the wheel, I'm not sure 
    /// which!</para>
    /// 
    /// <para>
    /// Here's the method, with a partial explanation for how it works:</para>
    /// 
    /// <para>
    /// We want to approximate a value m (given as a decimal) between 0 and 1, 
    /// by a fraction Y/X. Think of fractions as vectors (denominator, 
    /// numerator), so that the slope of the vector is the value of the 
    /// fraction. We are then looking for a lattice vector (X, Y) whose slope 
    /// is as close as possible to m. This picture illustrates the goal, and 
    /// shows that, given two vectors A and B on opposite sides of the desired 
    /// slope, their sum A + B = C is a new vector whose slope is between the 
    /// two, allowing us to narrow our search:</para>
    /// 
    /// <code>
    /// num
    /// ^
    /// |
    /// +  +  +  +  +  +  +  +  +  +  +
    /// |
    /// +  +  +  +  +  +  +  +  +  +  +
    /// |                                  slope m=0.7
    /// +  +  +  +  +  +  +  +  +  +  +   /
    /// |                               /
    /// +  +  +  +  +  +  +  +  +  +  D &lt;--- solution
    /// |                           /
    /// +  +  +  +  +  +  +  +  + /+  +
    /// |                       /
    /// +  +  +  +  +  +  +  C/ +  +  +
    /// |                   /
    /// +  +  +  +  +  + /+  +  +  +  +
    /// |              /
    /// +  +  +  +  B/ +  +  +  +  +  +
    /// |          /
    /// +  +  + /A  +  +  +  +  +  +  +
    /// |     /
    /// +  +/ +  +  +  +  +  +  +  +  +
    /// | /
    /// +--+--+--+--+--+--+--+--+--+--+--&gt; denom
    /// </code>
    /// 
    /// <para>
    /// Here we start knowing the goal is between A = (3,2) and B = (4,3), and 
    /// formed a new vector C = A + B. We test the slope of C and find that 
    /// the desired slope m is between A and C, so we continue the search 
    /// between A and C. We add A and C to get a new vector D = A + 2*B, which 
    /// in this case is exactly right and gives us the answer.</para>
    /// 
    /// <para>
    /// Given the vectors A and B, with slope(A) &lt; m &lt; slope(B), 
    /// we can find consecutive integers M and N such that 
    /// slope(A + M*B) &lt; x &lt; slope(A + N*B) in this way:</para>
    /// 
    /// <para>
    /// If A = (b, a) and B = (d, c), with a/b &lt; m &lt; c/d, solve</para>
    /// 
    /// <code>
    ///     a + x*c
    ///     ------- = m
    ///     b + x*d
    /// </code>
    /// 
    /// <para>
    /// to give</para>
    /// 
    /// <code>
    ///         b*m - a
    ///     x = -------
    ///         c - d*m
    /// </code>
    /// 
    /// <para>
    /// If this is an integer (or close enough to an integer to consider it 
    /// so), then A + x*B is our answer. Otherwise, we round it down and up to 
    /// get integer multipliers M and N respectively, from which new lower and 
    /// upper bounds A' = A + M*B and B' = A + N*B can be obtained. Repeat the 
    /// process until the slopes of the two vectors are close enough for the 
    /// desired accuracy. The process can be started with vectors (0,1), with 
    /// slope 0, and (1,1), with slope 1. Surprisingly, this process produces 
    /// exactly what continued fractions produce, and therefore it will 
    /// terminate at the desired fraction (in lowest terms, as far as I can 
    /// tell) if there is one, or when it is correct within the accuracy of 
    /// the original data.</para>
    /// 
    /// <para>
    /// For example, for the slope 0.7 shown in the picture above, we get 
    /// these approximations:</para>
    /// 
    /// <para>
    /// Step 1: A = 0/1, B = 1/1 (a = 0, b = 1, c = 1, d = 1)</para>
    /// 
    /// <code>
    ///         1 * 0.7 - 0   0.7
    ///     x = ----------- = --- = 2.3333
    ///         1 - 1 * 0.7   0.3
    /// 
    ///     M = 2: lower bound A' = (0 + 2*1) / (1 + 2*1) = 2 / 3
    ///     N = 3: upper bound B' = (0 + 3*1) / (1 + 3*1) = 3 / 4
    /// </code>
    /// 
    /// <para>
    /// Step 2: A = 2/3, B = 3/4 (a = 2, b = 3, c = 3, d = 4)</para>
    /// 
    /// <code>
    ///         3 * 0.7 - 2   0.1
    ///     x = ----------- = --- = 0.5
    ///         3 - 4 * 0.7   0.2
    /// 
    ///     M = 0: lower bound A' = (2 + 0*3) / (3 + 0*4) = 2 / 3
    ///     N = 1: upper bound B' = (2 + 1*3) / (3 + 1*4) = 5 / 7
    /// </code>
    /// 
    /// <para>
    /// Step 3: A = 2/3, B = 5/7 (a = 2, b = 3, c = 5, d = 7)</para>
    /// 
    /// <code>
    ///         3 * 0.7 - 2   0.1
    ///     x = ----------- = --- = 1
    ///         5 - 7 * 0.7   0.1
    /// 
    ///     N = 1: exact value A' = B' = (2 + 1*5) / (3 + 1*7) = 7 / 10
    /// </code>
    /// 
    /// <para>
    /// which of course is obviously right.</para>
    /// 
    /// <para>
    /// In most cases you will never get an exact integer, because of rounding 
    /// errors, but can stop when one of the two fractions is equal to the 
    /// goal to the given accuracy.</para>
    /// 
    /// <para>
    /// [...]Just to keep you up to date, I tried out my newly invented algorithm 
    /// and realized it lacked one or two things. Specifically, to make it 
    /// work right, you have to alternate directions, first adding A + N*B and 
    /// then N*A + B. I tested my program for all fractions with up to three 
    /// digits in numerator and denominator, then started playing with the 
    /// problem that affects you, namely how to handle imprecision in the 
    /// input. I haven't yet worked out the best way to allow for error, but 
    /// here is my C++ function (a member function in a Fraction class 
    /// implemented as { short num; short denom; } ) in case you need to go to 
    /// this algorithm.
    /// </para>
    /// 
    /// <para>[Edit [i_a]: tested a few stop criteria and precision settings;
    /// found that you can easily allow the algorithm to use the full integer
    /// value span: worst case iteration count was 21 - for very large prime
    /// numbers in the denominator and a precision set at double.Epsilon.
    /// Part of the code was stripped, then reinvented as I was working on a 
    /// proof for this system. For one, the reason to 'flip' the A/B treatment
    /// (i.e. the 'i&1' odd/even branch) is this: the factor N, which will
    /// be applied to the vector addition A + N*B is (1) an integer number to
    /// ensure the resulting vector (i.e. fraction) is rational, and (2) is
    /// determined by calculating the difference in direction between A and B.
    /// When the target vector direction is very close to A, the difference
    /// in *direction* (sort of an 'angle') is tiny, resulting in a tiny N
    /// value. Because the value is rounded down, A will not change. B will,
    /// but the number of iterations necessary to arrive at the final result
    /// increase significantly when the 'odd/even' processing is not included.
    /// Basically, odd/even processing ensures that once every second iteration
    /// there will be a major change in direction for any target vector M.]
    /// </para>
    /// 
    /// <para>[Edit [i_a]: further testing finds the empirical maximum
    /// precision to be ~ 1.0E-13, IFF you use the new high/low precision
    /// checks (simpler, faster) in the code (old checks have been commented out).
    /// Higher precision values cause the code to produce very huge fractions
    /// which clearly show the effect of limited floating point accuracy.
    /// Nevetheless, this is an impressive result.
    /// 
    /// I also changed the loop: no more odd/even processing but now we're
    /// looking for the biggest effect (i.e. change in direction) during EVERY
    /// iteration: see the new x1:x2 comparison in the code below.
    /// This will lead to a further reduction in the maximum number of iterations
    /// but I haven't checked that number now. Should be less than 21,
    /// I hope. ;-) ]
    /// </para>
    /// </summary>

	template<typename int_type>
	Fraction<int_type> toFract(double val, double Precision);

	template<typename int_type>
	Fraction<int_type> toFract(float val)
        {
            return toFract<int_type>(double(val), FLT_EPSILON /* 1.0E-13 */ /* float.Epsilon */ );
        }

	template<typename int_type>
	Fraction<int_type> toFract(double val)
        {
            return toFract<int_type>(val, DBL_EPSILON /* 1.0E-13 */ /* double.Epsilon */ );
        }

	template<typename int_type>
	Fraction<int_type> toFract(double val, double Precision)
		{
			constexpr int_type MaxValue{std::numeric_limits<int_type>::max()};

			if (DebugReporting) {
				std::cerr << std::format("Fraction: val = {}, precision = {}\n", val, Precision);
			}

			// find nearest fraction
			int_type intPart = int_type(val);
			val -= double(intPart);
			if (std::abs(val) > 1)
			{
				throw std::invalid_argument(std::format("fraction cannot be larger than +/-{}.", MaxValue));
			}

			Fraction<int_type> low(int_type(0), int_type(1));           // "A" = 0/1 (a/b)
			Fraction<int_type> high(int_type(1), int_type(1));          // "B" = 1/1 (c/d)

			if (DebugReporting) {
				std::cerr << std::format("Fraction: val = {}, precision = {}, intpart = {}\n", val, Precision, intPart);
			}

			for (;;)
			{
				assert(toFloat<int_type>(low) <= val);
				assert(toFloat<int_type>(high) >= val);

				//         b*m - a
				//     x = -------
				//         c - d*m
				double testLow = low.denominator() * val - low.numerator();
				double testHigh = high.numerator() - high.denominator() * val;

				if (DebugReporting)
				{
					std::cerr << std::format("Fraction: testlow = {} (fraction: {}), testhigh = {} (fraction: {})\n",
							testLow, low, testHigh, high);
				}

				// test for match:
				// 
				// m - a/b < precision
				//
				// ==>
				//
				// b * m - a < b * precision
				//
				// which is happening here: check both the current A and B fractions.
				//if (testHigh < high.denominator() * Precision)
				if (testHigh < Precision) // [i_a] speed improvement; this is even better for irrational 'val'
				{
					break; // high is answer
				}
				//if (testLow < low.denominator() * Precision)
				if (testLow < Precision) // [i_a] speed improvement; this is even better for irrational 'val'
				{
					// low is answer
					high = low;
					break;
				}

				double x1 = testHigh / testLow;
				double x2 = testLow / testHigh;

				if (DebugReporting) {
					std::cerr << std::format("Fraction: x1 = {}, x2 = {}, fraction = {}\n", x1, x2, high);
				}

				// always choose the path where we find the largest change in direction:
				if (x1 > x2)
				{
					//double x1 = testHigh / testLow;
					// safety checks: are we going to be out of integer bounds?
					if ((x1 + 1) * low.denominator() + high.denominator() >= double(MaxValue))
					{
						break;
					}

					int_type n = int_type(x1);    // lower bound for m
					//int m = n + 1;    // upper bound for m

					//     a + x*c
					//     ------- = m
					//     b + x*d
					int_type h_num = n * low.numerator() + high.numerator();
					int_type h_denom = n * low.denominator() + high.denominator();

					//int_type l_num = m * low.numerator() + high.numerator();
					//int_type l_denom = m * low.denominator() + high.denominator();
					int_type l_num = h_num + low.numerator();
					int_type l_denom = h_denom + low.denominator();

					if (DebugReporting) {
						std::cerr << std::format("Fraction: x1 LT x2: n = {}, h: {}/{}, l: {}/{}</p>\n", n, h_num, h_denom, l_num, l_denom);
					}

					low = Fraction<int_type>(l_num, l_denom);
					high = Fraction<int_type>(h_num, h_denom);
				}
				else
				{
					//double x2 = testLow / testHigh;
					// safety checks: are we going to be out of integer bounds?
					if (low.denominator() + (x2 + 1) * high.denominator() >= double(MaxValue))
					{
						break;
					}

					int_type n = int_type(x2);    // lower bound for m
					//int_type m = n + 1;    // upper bound for m

					//     a + x*c
					//     ------- = m
					//     b + x*d
					int_type l_num = low.numerator() + n * high.numerator();
					int_type l_denom = low.denominator() + n * high.denominator();

					//int_type h_num = low.numerator() + m * high.numerator();
					//int_type h_denom = low.denominator() + m * high.denominator();
					int_type h_num = l_num + high.numerator();
					int_type h_denom = l_denom + high.denominator();

					if (DebugReporting) {
						std::cerr << std::format("Fraction: x1 LT x2: n = {}, h: {}/{}, l: {}/{}\n", n, h_num, h_denom, l_num, l_denom);
					}

					high = Fraction<int_type>(h_num, h_denom);
					low = Fraction<int_type>(l_num, l_denom);
				}
				assert(toFloat(low) <= val);
				assert(toFloat(high) >= val);
			}

			high += intPart;  // high = fraction(high) + fraction(intPart / 1)

			if (DebugReporting)
			{
				std::cerr << std::format("Fraction: DONE for {} at precision {}: answer = {}\n", val, Precision, high);
			}

			return high;
		}
}



template<typename int_type>
struct std::formatter<boost::rational::rational<int_type>> {
	using MyType = boost::rational::rational<int_type>;

	constexpr auto parse(std::format_parse_context& ctx) {
		return ctx.begin();
	}

	auto format(const MyType& obj, std::format_context& ctx) const {
		return std::format_to(ctx.out(), "{}/{}", obj.numerator(), obj.denominator());
	}
};

#if 0
template<typename int_type>
struct std::formatter<cvt_2_fraction::Fraction<int_type>> {
	using MyType = cvt_2_fraction::Fraction<int_type>;

	constexpr auto parse(std::format_parse_context& ctx) {
		return ctx.begin();
	}

	auto format(const MyType& obj, std::format_context& ctx) const {
		return std::format_to(ctx.out(), "{}", toString(obj));
	}
};
#endif






/*
Original text: (https://web.archive.org/web/20181018112004/http://mathforum.org/library/drmath/view/51886.html)
===============================================================================================================

Date: 06/25/98 at 21:13:06
From: Doug Lawson
Subject: Decimal To Fraction Conversion

Dr. Math,

I am trying to find a method (which can be programmed on a PC) to 
convert the decimal part of a real number to a fraction represented by 
integers for the numerator and denominator. The fractional number has 
particular uses in the textile industry.

I have tried to think of some way to have two equations with two 
unknowns, but I just can't think of how to do this. What we do know 
is a = b/c. We know that 'a' is a decimal number and both 'b' and 'c' 
are integers. I have asked family, friends and coworkers... but no 
one knows how to solve this easily.

As an example, consider the following number:  0.047619
0.047619 = b/c 

Depending on the accuracy you wish to have, this can be represented 
by 0.047619 = 3/63

The problem is: how do you get there with formulas?

Please let me know if you can help.
Sincerely,
Doug Lawson
Leesona Div., Kvaerner U.S. Inc.

Date: 06/26/98 at 09:22:47
From: Doctor Jerry
Subject: Re: Decimal To Fraction Conversion

Hi Doug,

Any terminating decimal (1/3=0.33333... is not terminating) can be 
written as a fraction.  For example,

0.047619 
          = 0/10 + 4/100 + 7/1000 + 6/10000 + 1/100000 + 9/1000000 
          = 47619/1000000.

In some cases, the numerator and denominator will have common factors 
that can be removed. In the case you gave, there are no common 
factors.

I suspect you may be wanting a good approximation to a number like 
0.047619 in terms of fractions like x/16 or y/32. If this is the case, 
you could write a short program which runs through the numbers j/32, 
j=0,1,...,31, picking out the one closest to 0.047619.

- Doctor Jerry, The Math Forum
  http://mathforum.org/dr.math/   

Date: 06/26/98 at 13:28:05
From: Doctor Peterson
Subject: Re: Decimal To Fraction Conversion

Hi, Doug -

I saw Dr. Jerry's answer, and want to suggest another way that may be 
what you want. His idea is probably the best if what you want is, say, 
to convert lengths into standard fractions such as 32nds. But there is 
a standard way to approximate any decimal (even an irrational number 
like pi!) by a fraction as closely as you want. A nice feature of this 
method is that if your number is the exact value of a fraction, you 
will know when you have reached it. (Though it generally won't work 
exactly because you will be missing some decimal places.) What you do 
is to repeatedly take the reciprocal and remove the integer part, 
building what is called a "continued fraction" that is equal to your 
number. I'll demonstrate with your case first:

What I wrote down:           What I did on my calculator:

               1
0.047619 = ---------         Enter 0.047619 and hit the 1/x key
           21.000021

               1
         = ----------
                  1          Write down the 21 and subtract it
           21 + -----          to get 0.000021
                47619        Hit 1/x again

            1
         = --                Since 1/47619 is so small, ignore it
           21

Let's do a slightly more difficult example, 0.127659:

               1
0.127659 = ---------         Enter 0.127659 and hit 1/x
           7.8333686

                 1
         = ------------
                  1          Write down the 7 and subtract it
           7 + --------        to get 0.8333686
               1.199949      Hit 1/x again

                  1
         = ---------------
                    1
           7 + -----------
                      1      Write down the 1 and subtract it
               1 + -------     to get 0.199949
                   5.00126   Hit 1/x again

                  1
         = ---------------
                     1
           7 + -----------
                      1
               1 + -------
                        1    Write down the 5 and subtract it to get
                   5 + ---     0.00126
                       787

               1
         = ---------
                 1
           7 + -----
                   1
               1 + -
                   5         Since 1/787 is so small, call it zero and 
stop

This is the answer. All I have to do is simplify:

              1
         = -------
                1
           7 + ---
                6
                -
                5

             1
         = -----
               5
           7 + -
               6

             1
         = -----
            47
            ---
             6

            6
         = --
           47

This is the fraction I used to get the decimal. This method of 
continued fractions, incidentally, is also used to solve Diophantine 
(integer) equations, so your first inclination might have taken to 
this method if you had searched far enough.

In some cases you will have to arbitrarily decide when to stop, 
because there won't be a natural stopping point. For example, try the 
method on 0.7071068 (1/sqrt(2)).

The only part about this method that may be hard to program would be 
the simplification of the fraction. If you actually want to do it, I 
have a different version of this method that is a little harder to 
explain, and takes more steps on a calculator, but would be easier to 
program.

This may be more than you need, but it's so much fun I wanted you to 
see it.

- Doctor Peterson, The Math Forum
  http://mathforum.org/dr.math/   

Date: 06/28/98 at 21:12:22
From: Douglas Keith Lawson
Subject: Reply to Dr. Peterson Re: Decimal To Fraction Conversion

Thank you Dr. Peterson,

I will try your method at work and see how I can best implement it.  
I believe that your closing remarks in your reply to me were correct.  
The difficult part of the program would be simplification of the 
fraction. As such, I would be interested in seeing your alternate 
method.

To bore you with details about my interest in fractional math, let me
explain succinctly what I am doing. In the textile industry a type of
machine called a 'take-up' or 'winder' is used to wind fibers into 
free-standing cylindrical packages (like spools but without end 
plates). This is achieved using a thread guide that cycles parallel to 
the rotating axis of the cylinder being formed. The ratio of package 
revolutions to the axial traversing of the thread guide is defined as 
the wind ratio. Its significance in winding a good package can not be 
overstressed. In fact, this wind ratio is typically calculated and 
maintained with fixed gearing out to six (6) decimal places. 

The numerator and denominator of the fraction defining the wind ratio 
indicate a lot about how the fiber will be wound on the package. 
At first glance one would assume that this would not be critical; 
however, errors in the sixth decimal place can be seen with the naked 
eye and they can be detrimental to package stability.  

My firm has worked with the geared arangement for years and has easily 
been able to calculate the wind ratio as a function of gear ratios.  
As such, the numerator of the wind ratio is the product of several 
gear teeth multiplied by each other. The denominator is another gear 
tooth product. This method of calculation directly from given gear 
tooth numbers allowed us to keep the wind ratio as a fraction and 
easily utilize it that way. We are developing a digital control system 
to replace the gearing. We will enter the wind ratio directly on the 
computer controls of the winding machine as a number to six decimal 
places. This gives us greater flexibility than the gear arrangement; 
however, we now need a method to calculate wind parameters. What I 
need to do is take a given ratio and be able to return it to 
fractional form.

Your suggestions and your methods of approximating fractions are 
greatly appreciated.  I would be interested in seeing your alternate 
method of calculation and look forward to your reply.  Thank you again 
for your assistance.

Sincerely,
Doug Lawson
Leesona Div., Kvaerner U.S. Inc.

Date: 06/29/98 at 13:12:44
From: Doctor Peterson
Subject: Re: Decimal To Fraction Conversion

Hi, Doug. Now that I know how you plan to use the fraction, it's clear 
that my approach is what you need. 

The algorithm I am about to show you has an interesting history. I 
recently had a discussion with a teacher in England who had a 
challenging problem he had given his students, and wanted to know what 
others would do to solve it. The problem was to find the fraction 
whose decimal value he gave them, which is essentially identical to 
your problem! I wasn't familiar with a standard way to do it, but 
solved it by a vaguely remembered Diophantine method. Then, my 
curiosity piqued, and I searched the Web for information on the 
problem and didn't find it mentioned in terms of finding the fraction 
for an actual decimal, but as a way to approximate an irrational by a 
fraction, where the continued fraction method was used. 

I wrote to the teacher, and he responded with a method a student of 
his had come up with, which uses what amounts to a binary search 
technique. I recognized that this produced the same sequence of 
approximations that continued fractions gave, and was able to 
determine that it is really equivalent, and that it is known to some 
mathematicians (or at least math historians). 

After your request made me realize that this other method would be 
easier to program, I thought of an addition to make it more efficient, 
which to my knowledge is entirely new. So we're either on the cutting 
edge of computer technology or reinventing the wheel, I'm not sure 
which!

Here's the method, with a partial explanation for how it works:

We want to approximate a value m (given as a decimal) between 0 and 1, 
by a fraction Y/X. Think of fractions as vectors (denominator, 
numerator), so that the slope of the vector is the value of the 
fraction. We are then looking for a lattice vector (X, Y) whose slope 
is as close as possible to m. This picture illustrates the goal, and 
shows that, given two vectors A and B on opposite sides of the desired 
slope, their sum A + B = C is a new vector whose slope is between the 
two, allowing us to narrow our search:

num
^
|
+  +  +  +  +  +  +  +  +  +  +
|
+  +  +  +  +  +  +  +  +  +  +
|                                  slope m=0.7
+  +  +  +  +  +  +  +  +  +  +   /
|                               /
+  +  +  +  +  +  +  +  +  +  D <--- solution
|                           /
+  +  +  +  +  +  +  +  + /+  +
|                       /
+  +  +  +  +  +  +  C/ +  +  +
|                   /
+  +  +  +  +  + /+  +  +  +  +
|              /
+  +  +  +  B/ +  +  +  +  +  +
|          /
+  +  + /A  +  +  +  +  +  +  +
|     /
+  +/ +  +  +  +  +  +  +  +  +
| /
+--+--+--+--+--+--+--+--+--+--+--> denom

Here we start knowing the goal is between A = (3,2) and B = (4,3), and 
formed a new vector C = A + B. We test the slope of C and find that 
the desired slope m is between A and C, so we continue the search 
between A and C. We add A and C to get a new vector D = A + 2*B, which 
in this case is exactly right and gives us the answer.

Given the vectors A and B, with slope(A) < m < slope(B), 
we can find consecutive integers M and N such that 
slope(A + M*B) < x < slope(A + N*B) in this way:

If A = (b, a) and B = (d, c), with a/b < m < c/d, solve

    a + x*c
    ------- = m
    b + x*d

to give

        b*m - a
    x = -------
        c - d*m

If this is an integer (or close enough to an integer to consider it 
so), then A + x*B is our answer. Otherwise, we round it down and up to 
get integer multipliers M and N respectively, from which new lower and 
upper bounds A' = A + M*B and B' = A + N*B can be obtained. Repeat the 
process until the slopes of the two vectors are close enough for the 
desired accuracy. The process can be started with vectors (0,1), with 
slope 0, and (1,1), with slope 1. Surprisingly, this process produces 
exactly what continued fractions produce, and therefore it will 
terminate at the desired fraction (in lowest terms, as far as I can 
tell) if there is one, or when it is correct within the accuracy of 
the original data.

For example, for the slope 0.7 shown in the picture above, we get 
these approximations:

Step 1: A = 0/1, B = 1/1 (a = 0, b = 1, c = 1, d = 1)

        1 * 0.7 - 0   0.7
    x = ----------- = --- = 2.3333
        1 - 1 * 0.7   0.3

    M = 2: lower bound A' = (0 + 2*1) / (1 + 2*1) = 2 / 3
    N = 3: upper bound B' = (0 + 3*1) / (1 + 3*1) = 3 / 4

Step 2: A = 2/3, B = 3/4 (a = 2, b = 3, c = 3, d = 4)

        3 * 0.7 - 2   0.1
    x = ----------- = --- = 0.5
        3 - 4 * 0.7   0.2

    M = 0: lower bound A' = (2 + 0*3) / (3 + 0*4) = 2 / 3
    N = 1: upper bound B' = (2 + 1*3) / (3 + 1*4) = 5 / 7

Step 3: A = 2/3, B = 5/7 (a = 2, b = 3, c = 5, d = 7)

        3 * 0.7 - 2   0.1
    x = ----------- = --- = 1
        5 - 7 * 0.7   0.1

    N = 1: exact value A' = B' = (2 + 1*5) / (3 + 1*7) = 7 / 10

which of course is obviously right.

In most cases you will never get an exact integer, because of rounding 
errors, but can stop when one of the two fractions is equal to the 
goal to the given accuracy.

I have been playing with these ideas on my own, but have not yet 
gotten around to coding it to work out any bugs in the algorithm. I'll 
do that sometime soon, but you should be able to work it out from what 
I've given you. The only thing I'm really not sure of is the best way 
to decide when to stop. 

I hope this helps you. I have no special use for the algorithm right 
now (though I am a software engineer for a process control company!), 
but it's been fun playing with it.

- Doctor Peterson, The Math Forum
  http://mathforum.org/dr.math/   

Date: 07/01/98 at 21:22:55
From: Douglas Keith Lawson
Subject: Reply To Dr.Peterson Re: Decimal To Fraction Conversion

Hello again Dr. Peterson:

I have printed out your 'alternate' method regarding converting a 
decimal back to its fractional equivalent.  I will study it at length 
shortly.

Since I last wrote you I took your first explanation concerning the 
method of continuing fractions and studied it. On my PC at work I 
utilize MathCad 7.0 from MathSoft to solve several engineering 
problems. I decided to give it a try as a method of programming the 
continuing fractions method.

I did have to adjust my thinking to make the method conform to 
'textile math'; however, I was able to write a crude program which 
could determine the fractional equivalent to the decimal number 
accurately out to 9 decimal places. In the textile equivalent of the 
continuing fractions method it appears to be necessary to examine the 
numbers in question with regard to 'rounding'.  In short, if the 
number in question is 12.75, it is necessary to regard the fractional 
number as 13-1/4, not 12+3/4. This is necessary as the integer portion 
of the real number (i.e. 13 from the example above) describes the 
pattern formed by the fibers as they are wound on a cylinder. The 
subsequent integer numbers extracted from the continuing fractions
describe sub-patterns. These numbers are referred to as knuckle 
patterns and they influence how the wound fibers will lock together 
between wound layers.

I still have a lot of work to do, but I would be nowhere without your 
help. I might end up just refining what I wrote based on the 
continuing fractions method. I suppose your comments were correct, I 
am reinventing the wheel, at least to a degree. As I mentioned in my 
last e-mail, the traditional method of determining the parameters I am 
studying begins by multiplying the number of gear teeth into products 
defining the numerator and denominator of a fraction. The electronic 
controls we are developing may appear to give the same number to six 
decimal places, but what the continuing fractions method has made me 
aware of is the fact that errors out as far as 9 decimal places can 
significantly influence the physical results. I would not have 
believed it.

Again, I thank you for your help and I apologize for taking so much of 
your time.  I know that the primary avenue for this forum is 
supporting students. Again, I would not be anywhere with this if it 
were not for your help.

Sincerely,
Doug Lawson
Leesona Div., Kvaerner U.S. Inc.

Date: 07/04/98 at 12:20:48
From: Doctor Peterson
Subject: Re: Decimal To Fraction Conversion

Hi, Doug. Glad I can help - and it's especially nice to know you're 
not just using the methods but also the ideas I came up with! I've 
been having a lot of fun with this (since before your question), and 
I'm learning myself.

Just to keep you up to date, I tried out my newly invented algorithm 
and realized it lacked one or two things. Specifically, to make it 
work right, you have to alternate directions, first adding A + N*B and 
then N*A + B. I tested my program for all fractions with up to three 
digits in numerator and denominator, then started playing with the 
problem that affects you, namely how to handle imprecision in the 
input. I haven't yet worked out the best way to allow for error, but 
here is my C++ function (a member function in a Fraction class 
implemented as { short num; short denom; } ) in case you need to go to 
this algorithm:

Fraction Fraction::toFract(double val)
{    // find nearest fraction
 int intPart = (int)val;
 val -= (double)intPart;
 Fraction low(0, 1);           // "A" = 0/1
 Fraction high(1, 1);          // "B" = 1/1
 for (int i = 0; i < 100; ++i)
 {
  double testLow = low.denom * val - low.numerator();
  double testHigh = high.numerator() - high.denom * val;
  if (testHigh < Precision * high.denom)
   break; // high is answer
  if (testLow < Precision * low.denom)
  {  // low is answer
   high = low;
   break;
  }
  if (i & 1)
  {  // odd step: add multiple of low to high
   double test = testHigh / testLow;
   int count = (int)test;    // "N"
   int num = (count + 1) * low.numerator() + high.numerator();
   int denom = (count + 1) * low.denom + high.denom;
   if ((num > 0x8000) ||
       (denom > 0x10000))
    break;
   high.numerator() = num - low.numerator();  // new "A"
   high.denom = denom - low.denom;
   low.numerator() = num;             // new "B"
   low.denom = denom;
  }
  else
  {  // even step: add multiple of high to low
   double test = testLow / testHigh;
   int count = (int)test;     // "N"
   int num = low.numerator() + (count + 1) * high.numerator();
   int denom = low.denom + (count + 1) * high.denom;
   if ((num > 0x10000) ||
       (denom > 0x10000))
    break;
   low.numerator() = num - high.numerator();  // new "A"
   low.denom = denom - high.denom;
   high.numerator() = num;            // new "B"
   high.denom = denom;
  }
 }
 return Fraction(intPart, 1) + high;
}

I'm curious about one more thing: Would it be possible in your system 
to have the user enter a fraction rather than a decimal, to avoid the 
loss of precision? I recently answered a student's query as to whether 
decimals or fractions are "more precise," and my answer was that if 
the value you want is an exact fraction, you should represent it as a 
fraction, and if it's just a measurement with some level of precision, 
you should use a decimal. This could be a good application of that 
idea.

Thanks for keeping me informed. Just knowing this stuff is useful 
encourages me in helping students!

- Doctor Peterson, The Math Forum
  http://mathforum.org/dr.math/   

Date: 07/06/98 at 21:59:49
From: Douglas Keith Lawson
Subject: Reply to Dr. Peterson Re: Decimal To Fraction Conversion

Hello again Dr. Peterson,

Thank you for your last reply to my fraction question.  After I 
reviewed your 'slope' method of determining the fractional equivalent 
to a decimal number I have decided that your first suggestion of 
continuing fractions is best for my application. Both have merit; 
however, as I mentioned in my last correspondence, the integer numbers 
extracted from the continuing fractions method have meaning in the 
problem I am studying.

If I chose to examine the number by the slope method I might not 
obtain the same fraction and therefore might not extract the same 
integers that I can extract with the continuing fractions method.  
Your comments are well understood though, as far as determining 'how 
close' is close enough and knowing when to stop. 

I must apologize that your c++ code will not be of much use to me.  My
programming experience includes only FORTRAN and Pascal. In fact, I 
was writing a Pascal program to handle the continuing fractions. My 
Pascal problems are complicated by the fact that I only have a copy of 
Turbo Pascal 5.0 from Borland. Unfortunately, this was written for DOS 
and Win95 does not like the fact that Turbo makes direct system calls.  
As such I have to reboot in DOS mode to write my code. I was so 
frustrated that I downloaded an older, simpler Pascal compiler to 
write my code in a DOS window. I can't read or write to files with 
this version of Pascal, but once I get the program right I will create 
an executable file (EXE) using Turbo Pascal.

Once I realized what information I could extract using continuing 
fractions I knew this was the way to go.  The problem is as you 
suggested... writing the code to do this.

Thanks again for your help.

Sincerely,
Doug Lawson
Leesona Div., Kvaerner U.S. Inc.

Date: 03/22/15 at 12:46:45
From: Bart Broersma
Subject: Reply to Dr. Peterson Re: Decimal To Fraction Conversion

Hi,

I am one of the devolopers for the Lazarus IDE, and I've ported the C++
code on this page to Pascal.

I have adapted the code to handle 64-bit integers, negative numbers,
negative precisions, and bordeline values (values less than 0.5/MaxInt64
will return 0).

The code raises an exception if the input value is out of range for 64-bit
integer.

I have bound the precision parameter to the value parameter, in such a way
that value/precision will never be greater than 1E15, since greater
precision does not make much sense when using doubles.

The smallest possible precision can ever be 1/MaxInt64.

I have adjusted the test to see if a retrun candidate was inside the
specified precision (which may have to do more with the way I interpret
precision). With the original test, there were returnvalues that were
larger than the precision off (most of the time, just 1 significant digit
more).

I have thrown over 600 million random doubles (range: 0.0 - 1.0) to the
function and tested the results. They were all inside the (adjusted)
precision, and there were no mathematical overflows or rangecheck errors.

This of course does not prove that the function will never return a value
that does not match the precision, nor that it will never crash. For the
moment, however, I am satisfied that it works well.

I have applied the same testing to some other algorithms and they did
return fractions that did not match the precision, or suffered from
arithmetic overflow (which required extra checks inside that code).

I have now included the adapted code in my fractions unit, and I have put
in a comment:

{
  This implementation of FloatToFraction was originally written by:
  David Peterson and The Math Forum @ Drexel
  Source: http://mathforum.org/library/drmath/view/51886.html
  It was ported to FreePascal by Bart Broersma
  Adjustments made:
    * Precision is bound by a magnitude of -15 to Value
    * Bordeline cases close to zero are handled
    * Handle negative values
    * Handle negative precision
    * Original code dealt with 32-bit integers, it was adjusted for
64-bit integers

  The original copyright holder has granted me permission to adjust and
redistribute
  this code under LGPL license.

  When redistributing this code, the comments above MUST also be
redistributed with it!
}

(Note: "{" and "}" in pascal start and end comments)

===============================
procedure AdjustPrecision(var Precision: Double; Value: Double);
const
  MaxPrec = Double(1.0)/MaxInt64;
begin
  Precision := Abs(Precision);
  if ((Abs(Value) / Precision) > 1E15) then
  begin
    //writeln('Value / Precision > 1E15');
    Precision := Abs(Value) / 1E16;
  end;
  if (Precision < MaxPrec) then
    Precision := MaxPrec;
end;

procedure CheckRange(Value: Double);
begin
  if (Value > MaxInt64) or (Value < MinInt64) then
    raise ERangeError.Create(SRangeCheckError);
end;

function IsBorderlineValue(Value: Double; out F: TFraction): Boolean;
const
  MaxPrec = Double(1.0)/MaxInt64;
  ZeroBoundary = MaxPrec / 2;
begin
  if (Abs(Value) <= MaxPrec) then
  begin
    Result := True;
    //writeln('Abs(Value) < 1/MaxInt64 [',MaxPrec,']');
    if (Abs(Value) < ZeroBoundary) then
    begin
      //writeln('Abs(Value) < ZeroBoundary [',ZeroBoundary,']');
      F.Numerator := 0;
      F.Denominator := 1;
    end
    else
    begin
      if (Value < 0) then
      F.Numerator := -1
        else
      F.Numerator := 1;
      F.Denominator := MaxInt64;
    end;
  end
  else
    Result := False;
end;

function MF_FloatToFraction(Value, Precision: Double): TFraction;
var
  IntPart, Count, Num, Denom: Int64;
  i: Integer;
  TestLow, TestHigh, Test: Double;
  L,H: TFraction;
  IsNeg: Boolean;
begin // find nearest fraction
  CheckRange(Value);
  AdjustPrecision(Precision, Value);
  //Borderline cases
  if IsBorderlineValue(Value, Result) then
    Exit;
  IsNeg := (Value < 0);
  Value := Abs(Value);
  intPart := Round(Int(Value));
  Value := Frac(Value);
  L.Numerator := 0;
  L.Denominator := 1;
  H.Numerator := 1;
  H.denominator := 1;
  for i := 1 to 100 do
  begin
    testLow := L.Denominator * Value - L.Numerator;
    testHigh := H.Numerator - H.Denominator * Value;
    if (Abs(H.ToFloat - Value) < Precision) then
    begin
      break; // high is answer
    end;
    if (Abs(L.ToFloat - Value) < Precision) then
    begin // low is answer
      H := L;
      break;
    end;
    if Odd(i) then
    begin // odd step: add multiple of low to high
      test := testHigh / testLow;
      count := Round(Int(test)); // "N"
      num := (count + 1) * L.Numerator + H.Numerator;
      denom := (count + 1) * L.Denominator + H.Denominator;
      if ((num > High(Int64) - 1) or
      (denom > High(Int64) - 1)) then
      begin
        break;
      end;
      H.Numerator := num - L.Numerator; // new "A"
      H.Denominator := denom - L.Denominator;
      L.Numerator := num; // new "B"
      L.Denominator := denom;
    end
    else
    begin // even step: add multiple of high to low
      test := testLow / testHigh;
      count := Round(Int(test)); // "N"
      num := L.Numerator + (count + 1) * H.Numerator;
      denom := L.Denominator + (count + 1) * H.Denominator;
      if ((num > High(Int64) - 1) or
      (denom > High(Int64) - 1)) then
      begin
        //writeln('  ((num > High(Int64) - 1) or 
                    (denom > High(Int64) - 1))');
        break;
      end;
      L.Numerator := num - H.Numerator; // new "A"
      L.Denominator := denom - H.Denominator;
      H.Numerator := num; // new "B"
      H.Denominator := denom;
    end;
  end;
  //Avoid call to TFraction.Normalize in Result := H + IntPart
  Result := H;
  Result.Numerator := Result.Numerator+ (Result.Numerator * IntPart);
  if IsNeg then
    Result.Numerator := -Result.Numerator;
end;

*/
