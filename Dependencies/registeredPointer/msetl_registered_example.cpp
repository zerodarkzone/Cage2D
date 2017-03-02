
// Copyright (c) 2015 Noah Lopez
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//define MSE_SAFER_SUBSTITUTES_DISABLED /* This will replace all the classes with their native/standard counterparts. */

/* Each of the following will replace a subset of the classes with their native/standard counterparts. */
//define MSE_REGISTEREDPOINTER_DISABLED
//define MSE_SAFERPTR_DISABLED /* If you define MSE_SAFERPTR_DISABLED then you must also define MSE_REGISTEREDPOINTER_DISABLED. */
//define MSE_PRIMITIVES_DISABLED

#include "mseregistered.h"
#include "mserelaxedregistered.h"
#include <algorithm>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

/* This block of includes is required for the mse::TRegisteredRefWrapper example */
#include <algorithm>
#include <list>
#include <vector>
#include <iostream>
#include <numeric>
#include <random>
#include <functional>


int main(int argc, char* argv[])
{
	{
		/**************************/
		/*   TRegisteredPointer   */
		/**************************/

		/* For safety reasons we want to avoid using native pointers. "Managed" pointers like std:shared_ptr are an alternative, but
		sometimes you don't want a pointer that takes ownership (of the object's lifespan). So we provide mse::TRegisteredPointer.
		Because it doesn't take ownership, it can be used with objects allocated on the stack and is compatible with raii
		techniques. Also, in most cases, it can be used as a compatible, direct substitute for native pointers, making it
		straightforward to update legacy code. */

		class A {
		public:
			virtual ~A() {}
			int b = 3;
		};
		class B {
		public:
			static int foo1(A* a_native_ptr) { return a_native_ptr->b; }
			static int foo2(mse::TRegisteredPointer<A> A_registered_ptr) { return A_registered_ptr->b; }
			/* mse::TRegisteredFixedConstPointer<A> is recommended where you might have used "const A&".*/
			static int foo3(mse::TRegisteredFixedConstPointer<A> A_registered_fc_ptr) { return A_registered_fc_ptr->b; }
		protected:
			~B() {}
		};

		A* A_native_ptr = nullptr;
		/* mse::TRegisteredPointer<> is basically a "safe" version of the native pointer. */
		mse::TRegisteredPointer<A> A_registered_ptr1;

		{
			A a;
			mse::TRegisteredObj<A> registered_a;
			/* mse::TRegisteredObj<A> is a class that is publicly derived from A, and so should be a compatible substitute for A
			in almost all cases. */

			assert(a.b == registered_a.b);
			A_native_ptr = &a;
			A_registered_ptr1 = &registered_a;
			assert(A_native_ptr->b == A_registered_ptr1->b);

			mse::TRegisteredPointer<A> A_registered_ptr2 = &registered_a;
			A_registered_ptr2 = nullptr;
			try {
				int i = A_registered_ptr2->b; /* this is gonna throw an exception */
			}
			catch (...) {
				std::cerr << "expected exception" << std::endl;
				/* The exception is triggered by an attempt to dereference a null "registered pointer". */
			}

			/* mse::TRegisteredPointers can be coerced into native pointers if you need to interact with legacy code or libraries. */
			B::foo1((A*)A_registered_ptr1);

			B::foo3(&registered_a);
			/* mse::TRegisteredPointers don't convert directly into mse::TRegisteredFixedConstPointers because
			mse::TRegisteredFixedConstPointers are never supposed to be null, where mse::TRegisteredPointers can be. But you
			can easily obtain an mse::TRegisteredFixedPointer which does convert to an mse::TRegisteredFixedConstPointer. */
			B::foo3(&*A_registered_ptr1);

			if (A_registered_ptr2) {
			}
			else if (A_registered_ptr2 != A_registered_ptr1) {
				A_registered_ptr2 = A_registered_ptr1;
				assert(A_registered_ptr2 == A_registered_ptr1);
			}
		}

		try {
			/* A_registered_ptr1 "knows" that the (registered) object it was pointing to has now been deallocated. */
			int i = A_registered_ptr1->b; /* So this is gonna throw an exception */
		}
		catch (...) {
			std::cerr << "expected exception" << std::endl;
		}

		{
			/* For heap allocations mse::registered_new is kind of analagous to std::make_shared, but again,
			mse::TRegisteredPointers don't take ownership so you are responsible for deallocation. */
			auto A_registered_ptr3 = mse::registered_new<A>();
			assert(3 == A_registered_ptr3->b);
			mse::registered_delete<A>(A_registered_ptr3);
			try {
				/* A_registered_ptr3 "knows" that the (registered) object it was pointing to has now been deallocated. */
				int i = A_registered_ptr3->b; /* So this is gonna throw an exception */
			}
			catch (...) {
				std::cerr << "expected exception" << std::endl;
			}
		}

		{
			/***********************************/
			/*   TRelaxedRegisteredPointer   */
			/***********************************/

			/* mse::TRelaxedRegisteredPointer<> behaves very similar to mse::TRegisteredPointer<> but is even more "compatible"
			with native pointers (i.e. less explicit casting is required when interacting with native pointers and native pointer
			interfaces). So if you're updating existing or legacy code to be safer, replacing native pointers with
			mse::TRelaxedRegisteredPointer<> may be more convenient than mse::TRegisteredPointer<>.
			One case where you may need to use mse::TRelaxedRegisteredPointer<> even when not dealing with legacy code is when
			you need a reference to a class before it is fully defined. For example, when you have two classes that mutually
			reference each other. mse::TRegisteredPointer<> does not support this.
			*/

			class C;

			class D {
			public:
				virtual ~D() {}
				mse::TRelaxedRegisteredPointer<C> m_c_ptr;
			};

			class C {
			public:
				mse::TRelaxedRegisteredPointer<D> m_d_ptr;
			};

			mse::TRelaxedRegisteredObj<C> regobjfl_c;
			mse::TRelaxedRegisteredPointer<D> d_ptr = mse::relaxed_registered_new<D>();

			regobjfl_c.m_d_ptr = d_ptr;
			d_ptr->m_c_ptr = &regobjfl_c;

			mse::relaxed_registered_delete<D>(d_ptr);
		}

		mse::s_regptr_test1();
		mse::s_relaxedregptr_test1();

		{
			/*************************/
			/*   Simple Benchmarks   */
			/*************************/

			/* Just some simple speed tests. */
			class CE {
			public:
				CE() {}
				CE(int& count_ref) : m_count_ptr(&count_ref) { (*m_count_ptr) += 1; }
				virtual ~CE() { (*m_count_ptr) -= 1; }
				int m_x;
				int *m_count_ptr;
			};
#ifdef _DEBUG
			static const int number_of_loops = 10/*arbitrary*/;
#else /*_DEBUG*/
			static const int number_of_loops = 1000000/*arbitrary*/;
#endif /*_DEBUG*/
			std::cout << std::endl;
			std::cout << "Some simple benchmarks: \n";
			{
				int count = 0;
				auto item_ptr2 = new CE(count);
				delete item_ptr2; item_ptr2 = nullptr;
				auto t1 = std::chrono::high_resolution_clock::now();
				for (int i = 0; i < number_of_loops; i += 1) {
					auto item_ptr = new CE(count);
					item_ptr2 = item_ptr;
					delete item_ptr;
					item_ptr = nullptr;
				}

				auto t2 = std::chrono::high_resolution_clock::now();
				auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
				std::cout << "native pointer: " << time_span.count() << " seconds.";
				if (0 != count) {
					std::cout << " destructions pending: " << count << "."; /* Using the count variable for (potential) output should prevent the optimizer from discarding it. */
				}
				std::cout << std::endl;
			}
			{
				int count = 0;
				auto item_ptr2 = mse::registered_new<CE>(count);
				mse::registered_delete<CE>(item_ptr2);
				auto t1 = std::chrono::high_resolution_clock::now();
				for (int i = 0; i < number_of_loops; i += 1) {
					auto item_ptr = mse::registered_new<CE>(count);
					item_ptr2 = item_ptr;
					mse::registered_delete<CE>(item_ptr);
				}

				auto t2 = std::chrono::high_resolution_clock::now();
				auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
				std::cout << "mse::TRegisteredPointer: " << time_span.count() << " seconds.";
				if (0 != count) {
					std::cout << " destructions pending: " << count << "."; /* Using the count variable for (potential) output should prevent the optimizer from discarding it. */
				}
				std::cout << std::endl;
			}
			{
				int count = 0;
				auto item_ptr2 = mse::relaxed_registered_new<CE>(count);
				mse::relaxed_registered_delete<CE>(item_ptr2);
				auto t1 = std::chrono::high_resolution_clock::now();
				for (int i = 0; i < number_of_loops; i += 1) {
					auto item_ptr = mse::relaxed_registered_new<CE>(count);
					item_ptr2 = item_ptr;
					mse::relaxed_registered_delete<CE>(item_ptr);
				}

				auto t2 = std::chrono::high_resolution_clock::now();
				auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
				std::cout << "mse::TRelaxedRegisteredPointer: " << time_span.count() << " seconds.";
				if (0 != count) {
					std::cout << " destructions pending: " << count << "."; /* Using the count variable for (potential) output should prevent the optimizer from discarding it. */
				}
				std::cout << std::endl;
			}
			{
				int count = 0;
				auto item_ptr2 = std::make_shared<CE>(count);
				auto t1 = std::chrono::high_resolution_clock::now();
				for (int i = 0; i < number_of_loops; i += 1) {
					auto item_ptr = std::make_shared<CE>(count);
					item_ptr2 = item_ptr;
					item_ptr = nullptr;
				}
				item_ptr2 = nullptr;

				auto t2 = std::chrono::high_resolution_clock::now();
				auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
				std::cout << "std::shared_ptr: " << time_span.count() << " seconds.";
				if (0 != count) {
					std::cout << " destructions pending: " << count << "."; /* Using the count variable for (potential) output should prevent the optimizer from discarding it. */
				}
				std::cout << std::endl;
			}
			{
				int count = 0;
				auto item_ptr2 = &(mse::TRegisteredObj<CE>(count));
				auto t1 = std::chrono::high_resolution_clock::now();
				{
					for (int i = 0; i < number_of_loops; i += 1) {
						mse::TRegisteredObj<CE> object(count);
						auto item_ptr = &object;
						item_ptr2 = item_ptr;
					}
				}

				auto t2 = std::chrono::high_resolution_clock::now();
				auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
				std::cout << "mse::TRegisteredPointer targeting the stack: " << time_span.count() << " seconds.";
				if (0 != count) {
					std::cout << " destructions pending: " << count << "."; /* Using the count variable for (potential) output should prevent the optimizer from discarding it. */
				}
				std::cout << std::endl;
			}

			std::cout << std::endl;
			static const int number_of_loops2 = (10/*arbitrary*/)*number_of_loops;
			{
				class CF {
				public:
					CF(int a = 0) : m_a(a) {}
					CF* m_next_item_ptr;
					int m_a = 3;
				};
				CF item1(1);
				CF item2(2);
				CF item3(3);
				item1.m_next_item_ptr = &item2;
				item2.m_next_item_ptr = &item3;
				item3.m_next_item_ptr = &item1;
				auto t1 = std::chrono::high_resolution_clock::now();
				CF* cf_ptr = item1.m_next_item_ptr;
				for (int i = 0; i < number_of_loops2; i += 1) {
					cf_ptr = cf_ptr->m_next_item_ptr;
				}
				auto t2 = std::chrono::high_resolution_clock::now();
				auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
				std::cout << "native pointer dereferencing: " << time_span.count() << " seconds.";
				if (3 == cf_ptr->m_a) {
					std::cout << " "; /* Using cf_ptr->m_a for (potential) output should prevent the optimizer from discarding too much. */
				}
				std::cout << std::endl;
			}
			{
				class CF {
				public:
					CF(int a = 0) : m_a(a) {}
					mse::TRelaxedRegisteredPointer<CF> m_next_item_ptr;
					int m_a = 3;
				};
				mse::TRelaxedRegisteredObj<CF> item1(1);
				mse::TRelaxedRegisteredObj<CF> item2(2);
				mse::TRelaxedRegisteredObj<CF> item3(3);
				item1.m_next_item_ptr = &item2;
				item2.m_next_item_ptr = &item3;
				item3.m_next_item_ptr = &item1;
				auto t1 = std::chrono::high_resolution_clock::now();
				mse::TRelaxedRegisteredPointer<CF>* rpfl_ptr = std::addressof(item1.m_next_item_ptr);
				for (int i = 0; i < number_of_loops2; i += 1) {
					rpfl_ptr = std::addressof((*rpfl_ptr)->m_next_item_ptr);
				}
				auto t2 = std::chrono::high_resolution_clock::now();
				auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
				std::cout << "mse::TRelaxedRegisteredPointer (checked) dereferencing: " << time_span.count() << " seconds.";
				if (3 == (*rpfl_ptr)->m_a) {
					std::cout << " "; /* Using rpfl_ref->m_a for (potential) output should prevent the optimizer from discarding too much. */
				}
				std::cout << std::endl;
			}
			{
				class CF {
				public:
					CF(int a = 0) : m_a(a) {}
					mse::TRelaxedRegisteredPointer<CF> m_next_item_ptr;
					int m_a = 3;
				};
				mse::TRelaxedRegisteredObj<CF> item1(1);
				mse::TRelaxedRegisteredObj<CF> item2(2);
				mse::TRelaxedRegisteredObj<CF> item3(3);
				item1.m_next_item_ptr = &item2;
				item2.m_next_item_ptr = &item3;
				item3.m_next_item_ptr = &item1;
				auto t1 = std::chrono::high_resolution_clock::now();
				CF* cf_ptr = (item1.m_next_item_ptr);
				for (int i = 0; i < number_of_loops2; i += 1) {
					cf_ptr = (cf_ptr->m_next_item_ptr);
				}
				auto t2 = std::chrono::high_resolution_clock::now();
				auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
				std::cout << "mse::TRelaxedRegisteredPointer unchecked dereferencing: " << time_span.count() << " seconds.";
				if (3 == cf_ptr->m_a) {
					std::cout << " "; /* Using rpfl_ref->m_a for (potential) output should prevent the optimizer from discarding too much. */
				}
				std::cout << std::endl;
			}
			{
				class CF {
				public:
					CF(int a = 0) : m_a(a) {}
					std::weak_ptr<CF> m_next_item_ptr;
					int m_a = 3;
				};
				auto item1_ptr = std::make_shared<CF>(1);
				auto item2_ptr = std::make_shared<CF>(2);
				auto item3_ptr = std::make_shared<CF>(3);
				item1_ptr->m_next_item_ptr = item2_ptr;
				item2_ptr->m_next_item_ptr = item3_ptr;
				item3_ptr->m_next_item_ptr = item1_ptr;
				auto t1 = std::chrono::high_resolution_clock::now();
				std::weak_ptr<CF>* wp_ptr = &(item1_ptr->m_next_item_ptr);
				for (int i = 0; i < number_of_loops2; i += 1) {
					wp_ptr = &((*wp_ptr).lock()->m_next_item_ptr);
				}
				auto t2 = std::chrono::high_resolution_clock::now();
				auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
				std::cout << "std::weak_ptr dereferencing: " << time_span.count() << " seconds.";
				if (3 == (*wp_ptr).lock()->m_a) {
					std::cout << " "; /* Using wp_ref.lock()->m_a for (potential) output should prevent the optimizer from discarding too much. */
				}
				std::cout << std::endl;
			}
		}
	}

#ifdef MSEREGISTEREDREFWRAPPER
	{
		/*****************************/
		/*   TRegisteredRefWrapper   */
		/*****************************/

		/* Stl provides a copyable, assignable wrapper for C++ references called std::reference_wrapper. std::reference_wrappers,
		like native C++ references, are not completely safe in the sense that the object they refer to can be deallocated while
		a reference to it is still available. So we provide mse::TRegisteredRefWrapper, a safe implementation of
		std::reference_wrapper that "knows" when the object being referenced has been deallocated and will throw an exception
		on any attempt to access the object after it has been destroyed.
		In most cases it is probably preferable to just use mse::TRegisteredFixedPointer instead of
		mse::TRegisteredRefWrapper. 
		*/
		{
			/* This example originally comes from http://en.cppreference.com/w/cpp/utility/functional/reference_wrapper. */
			std::list<mse::TRegisteredObj<mse::CInt>> l(10);
			std::iota(l.begin(), l.end(), -4);

			std::vector<mse::TRegisteredRefWrapper<mse::CInt>> v(l.begin(), l.end());
			// can't use shuffle on a list (requires random access), but can use it on a vector
			std::shuffle(v.begin(), v.end(), std::mt19937{ std::random_device{}() });

			std::cout << '\n';
			std::cout << "TRegisteredRefWrapper test output: \n";
			std::cout << "Contents of the list: ";
			for (auto n : l) { std::cout << n << ' '; } std::cout << '\n';

			std::cout << "Contents of the list, as seen through a shuffled vector: ";
			for (auto i : v) { std::cout << (mse::CInt&)i << ' '; } std::cout << '\n';

			std::cout << "Doubling the values in the initial list...\n";
			for (auto& i : l) {
				i *= 2;
			}

			std::cout << "Contents of the list, as seen through a shuffled vector: ";
			for (auto i : v) { std::cout << (mse::CInt&)i << ' '; } std::cout << '\n';
			std::cout << '\n';
		}
		{
			/* This example originally comes from http://www.cplusplus.com/reference/functional/reference_wrapper/. */
			mse::TRegisteredObj<mse::CInt> a(10), b(20), c(30);
			// an array of "references":
			mse::TRegisteredRefWrapper<mse::CInt> refs[] = { a,b,c };
			std::cout << "refs:";
			for (mse::CInt& x : refs) std::cout << ' ' << x;
			std::cout << '\n';

			mse::TRegisteredObj<mse::CInt> foo(10);
			auto bar = mse::registered_ref(foo);
			//++bar;
			//++(mse::TRegisteredObj<mse::CInt>&)bar;
			++(mse::CInt&)bar;
			std::cout << foo << '\n';
		}
	}
#endif // MSEREGISTEREDREFWRAPPER

	return 0;
}

