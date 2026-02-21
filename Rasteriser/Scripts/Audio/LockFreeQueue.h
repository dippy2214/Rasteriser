//	LockFreeQueue.h - Simple lock-free queue class.
//  ----------------------------------------------------------------------------
//	Copyright 2020 Niall Moody
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to
//	deal in the Software without restriction, including without limitation the
//	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//	sell copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//	IN THE SOFTWARE.
//	----------------------------------------------------------------------------

#pragma once

#include <atomic>

///	Simple lock-free queue class.
/*!
	This code is adapted from:
	http://www.drdobbs.com/parallel/writing-lock-free-code-a-corrected-queue/210604448?pgno=2

	Note that this code is designed for a single *producer* thread and a single
	*consumer* thread. It will not work in other situations.
 */
template <typename T>
class LockFreeQueue
{
public:
	///	Constructor.
	LockFreeQueue()
	{
		first = divider = last = new Node(T()); //Add dummy separator.
	}
	///	Destructor.
	~LockFreeQueue()
	{
		while(first != nullptr)
		{
			//Release the list.
			Node *tmp = first;

			first = tmp->next;
			delete tmp;
		}
	}

	///	Pushes a value onto the queue (from *producer* thread).
	void push(const T& t)
	{
		//Add the new item.
		((Node *)last)->next = new Node(t);
		//Publish it.
		last = ((Node *)last)->next;

		//Trim unused nodes.
		while(first != divider)
		{
			Node *tmp = first;

			first = first->next;
			delete tmp;
		}
	}

	///	Pops a value from the queue (from *consumer* thread).
	/*!
		\param result The value that was read (if there was one).

		\return True if we were able to read a value; false otherwise.
	 */
	bool pop(T& result)
	{
		//If queue is not empty.
		if(divider != last)
		{
			//Copy next value into return result.
			result = ((Node *)divider)->next->value;
			//Publish that we *consumed* that value.
			divider = ((Node *)divider)->next;
			return true;
		}

		return false;
	}

	///	Returns true if there are elements waiting in the queue.
	/*!
		Best to only call this from the *consumer* thread.
	 */
	bool elementsPending() const
	{
		return (divider != last);
	}
private:
	///	Struct representing a single node in the queue.
	struct Node
	{
		Node(T val):
		value(val),
		next(nullptr)
		{
		
		}

		///	The node's value
		T value;
		///	Pointer to the next node in the queue (if there is one).
		Node *next;
	};

	///	Accessed by *producer* only.
	Node *first;
	///	Shared between *producer* and *consumer*.
	std::atomic<Node *> divider;
	///	Shared between *producer* and *consumer*.
	std::atomic<Node *> last;
};
