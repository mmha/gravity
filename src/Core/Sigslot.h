#pragma once

// sigslot.h: Signal/Slot classes
//
// Based on the sigslot library
//
// QUICK DOCUMENTATION
//
//
//
//		THREADING MODES
//
//			single_threaded				- Your program is assumed to be single threaded from the point of view
//										  of signal/slot usage (i.e. all objects using signals and slots are
//										  created and destroyed from a single thread). Behaviour if objects are
//										  destroyed concurrently is undefined (i.e. you'll get the occasional
//										  segmentation fault/memory exception).
//
//			multi_threaded_global		- Your program is assumed to be multi threaded. Objects using signals and
//										  slots can be safely created and destroyed from any thread, even when
//										  connections exist. In multi_threaded_global mode, this is achieved by a
//										  single global mutex (actually a critical section on Windows because they
//										  are faster). This option uses less OS resources, but results in more
//										  opportunities for contention, possibly resulting in more context switches
//										  than are strictly necessary.
//
//			multi_threaded_local		- Behaviour in this mode is essentially the same as multi_threaded_global,
//										  except that each signal, and each object that inherits has_slots, all
//										  have their own mutex/critical section. In practice, this means that
//										  mutex collisions (and hence context switches) only happen if they are
//										  absolutely essential. However, on some platforms, creating a lot of
//										  mutexes can slow down the whole OS, so use this option with care.

#include <algorithm>
#include <vector>
#include <mutex>

namespace ge::Core
{
	enum LockPolicy
	{
		single_threaded,
		multi_threaded_global,
		multi_threaded_local
	};

	template<LockPolicy>
	class Guard;

	template<>
	class Guard<single_threaded>
	{
	};

	template<>
	class Guard<multi_threaded_local>
	{
		Guard()
		{
			mtx.lock();
		}

		~Guard()
		{
			mtx.unlock();
		}

		private:
		std::mutex mtx;
	};

	template<>
	class Guard<multi_threaded_global>
	{
		Guard()
		{
			mtx.lock();
		}
		~Guard()
		{
			mtx.unlock();
		}

		private:
		static std::mutex mtx;
	};

	class SignalBase
	{
		public:
		virtual void disconnectAllTo(void *object) = 0;
	};

	template<typename... Args>
	class Delegate;

	template <typename... Args>
	class Delegate<void(Args...)>
	{
		public:
		template<typename T>
		Delegate(T *object, void (T::*func)(Args...))
			: object_{object}
			, func{dirty_cast<T>{func}.cast_func}
		{
		}

		void operator()(Args... args)
		{
			func(object_, args...);
		}

		bool sameCallee(void *object) const
		{
			return object == object;
		}

		private:
		template<typename T>
		union dirty_cast
		{
			void (T::*memfunc)(Args...);
			void (*cast_func)(void *, Args...);
		};
		void *object_;
		void (*func)(void *, Args...);
	};

	class HasSlots
	{
		public:
		void registerSignal(SignalBase *signal)
		{
			connectedSignals.emplace_back(signal);
		}

		~HasSlots()
		{
			for(auto signal : connectedSignals)
			{
				signal->disconnectAllTo(this);
			}
		}

		private:
		std::vector<SignalBase *> connectedSignals;
	};

	template<LockPolicy lockPolicy, typename... Args>
	class Signal final : public SignalBase
	{
		public:
		void connect(auto &object, auto call)
		{
			Guard<lockPolicy> g;
			(void) g;
			connected_slots.emplace_back(&object, call);
			object.registerSignal(this);
		}

		template<typename T>
		void disconnect(T &object, void (T::*call)(Args...))
		{
			Guard<lockPolicy> g;
			connected_slots.erase(Delegate<void(Args...)>(object, call));
		}

		void disconnectAllTo(void *object) override
		{
			connected_slots.erase(
				std::remove_if(connected_slots.begin(),
				               connected_slots.end(),
				               [object](auto &delegate)
				{
					return delegate.sameCallee(object);
				}),
				connected_slots.end());
		}

		void operator()(auto &&... args)
		{
			for(auto &&slot : connected_slots)
			{
				slot(std::forward<Args>(args)...);
			}
		}

		private:
		std::vector<Delegate<void(Args...)>> connected_slots;
	};
}
