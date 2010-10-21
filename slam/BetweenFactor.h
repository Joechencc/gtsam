/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 *  @file  BetweenFactor.h
 *  @authors Frank Dellaert, Viorela Ila
 **/
#pragma once

#include <ostream>

#include <gtsam/nonlinear/NonlinearFactor.h>
#include <gtsam/linear/GaussianFactor.h>
#include <gtsam/base/Lie.h>
#include <gtsam/base/Matrix.h>

namespace gtsam {

	/**
	 * A class for a measurement predicted by "between(config[key1],config[key2])"
	 * T is the Lie group type, Values where the T's are gotten from
	 *
	 * FIXME: This should only need one key type, as we can't have different types
	 */
	template<class VALUES, class KEY1, class KEY2 = KEY1>
	class BetweenFactor: public NonlinearFactor2<VALUES, KEY1, KEY2> {

	public:
		typedef typename KEY1::Value T;

	private:

		typedef NonlinearFactor2<VALUES, KEY1, KEY2> Base;

		T measured_; /** The measurement */

	public:

		// shorthand for a smart pointer to a factor
		typedef typename boost::shared_ptr<BetweenFactor> shared_ptr;

		/** Constructor */
		BetweenFactor(const KEY1& key1, const KEY2& key2, const T& measured,
				const SharedGaussian& model) :
			Base(model, key1, key2), measured_(measured) {
		}

		/** implement functions needed for Testable */

		/** print */
		void print(const std::string& s) const {
			Base::print(s);
			measured_.print("measured");
		}

		/** equals */
		bool equals(const NonlinearFactor<VALUES>& expected, double tol) const {
			const BetweenFactor<VALUES, KEY1, KEY2> *e =
					dynamic_cast<const BetweenFactor<VALUES, KEY1, KEY2>*> (&expected);
			return e != NULL && Base::equals(expected, tol) && this->measured_.equals(
					e->measured_, tol);
		}

		/** implement functions needed to derive from Factor */

		/** vector of errors */
		Vector evaluateError(const T& p1, const T& p2, boost::optional<Matrix&> H1 =
				boost::none, boost::optional<Matrix&> H2 = boost::none) const {
			T hx = p1.between(p2, H1, H2); // h(x)
			// manifold equivalent of h(x)-z -> log(z,h(x))
			return measured_.logmap(hx);
		}

		/** return the measured */
		inline const T measured() const {
			return measured_;
		}

		/** number of variables attached to this factor */
		inline std::size_t size() const {
			return 2;
		}
	};

} /// namespace gtsam
