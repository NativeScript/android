#ifndef LRUCACHE_H_
#define LRUCACHE_H_

/*
 Copyright (c) 2010-2011, Tim Day <timday@timday.com>

 Permission to use, copy, modify, and/or distribute this software for any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <cassert>
#include <list>
#include <unordered_map>

namespace tns {
// Class providing fixed-size (by number of records)
// LRU-replacement cache of a function with signature
// V f(K).
// MAP should be one of std::map or std::unordered_map.
// Variadic template args used to deal with the
// different type argument signatures of those
// containers; the default comparator/hash/allocator
// will be used.
template<typename K, typename V>
class LRUCache {
    public:

        typedef K key_type;
        typedef V value_type;

        // Key access history, most recent at back
        typedef std::list<key_type> key_tracker_type;

        // Key to value and key history iterator
        typedef std::unordered_map< key_type, std::pair<value_type, typename key_tracker_type::iterator> > key_to_value_type;

        // Constuctor specifies the cached function and
        // the maximum number of records to be stored
        LRUCache(value_type (*loadCallback)(const key_type&, void*), void (*evictCallback)(const value_type&, void*), size_t capacity, void* state)
            : m_loadCallback(loadCallback), m_capacity(capacity), m_evictCallback(evictCallback), m_state(state) {
            assert(m_loadCallback != nullptr);
            assert((0 < m_capacity) && (m_capacity < 10000));
        }

        // Obtain value of the cached function for k
        value_type operator()(const key_type& k) {

            // Attempt to find existing record
            auto it = m_key_to_value.find(k);

            if (it == m_key_to_value.end()) {

                // We don't have it:

                // Evaluate function and create new record
                const value_type v = m_loadCallback(k, m_state);
                insert(k,v);

                // Return the freshly computed value
                return v;

            } else {
                // We do have it:

                // Update access record by moving
                // accessed key to back of list
                m_key_tracker.splice(m_key_tracker.end(), m_key_tracker, (*it).second.second);

                // Return the retrieved value
                return (*it).second.first;
            }
        }

        // Obtain the cached keys, most recently used element
        // at head, least recently used at tail.
        // This method is provided purely to support testing.
        template <typename IT> void get_keys(IT dst) const {
            auto src = m_key_tracker.rbegin();

            while (src != m_key_tracker.rend()) {
                *dst++ = *src++;
            }
        }

        void update(const key_type& key, const value_type& value) {
            jweak ref = m_loadCallback(key, m_state);
            insert(key, ref);
        }

    private:

        // Record a fresh key-value pair in the cache
        void insert(const key_type& k, const value_type& v) {
            // Method is only called on cache misses
            assert(m_key_to_value.find(k) == m_key_to_value.end());

            // Make space if necessary
            if (m_key_to_value.size() == m_capacity) {
                evict();
            }

            // Record k as most-recently-used key
            auto it = m_key_tracker.insert(m_key_tracker.end(), k);

            // Create the key-value entry,
            // linked to the usage record.
            m_key_to_value.insert(std::make_pair(k, std::make_pair(v, it)));
            // No need to check return,
            // given previous assert.
        }

        // Purge the least-recently-used element in the cache
        void evict() {
            // Assert method is never called when cache is empty
            assert(!m_key_tracker.empty());

            // Identify least recently used key
            auto it = m_key_to_value.find(m_key_tracker.front());
            assert(it != m_key_to_value.end());

            if (m_evictCallback != nullptr) {
                m_evictCallback((*it).second.first, m_state);
            }

            // Erase both elements to completely purge record
            m_key_to_value.erase(it);
            m_key_tracker.pop_front();
        }

        // The function to be cached
        value_type (*m_loadCallback)(const key_type&, void*);

        void (*m_evictCallback)(const value_type&, void*);

        // Maximum number of key-value pairs to be retained
        const size_t m_capacity;

        // Key access history
        key_tracker_type m_key_tracker;

        // user-defined state to pass to callback
        void* m_state;

        // Key-to-value lookup
        key_to_value_type m_key_to_value;
};
}

#endif /* LRUCACHE_H_ */
