#ifndef _SINGLETON_H_
#define _SINGLETON_H_
#include <boost/noncopyable.hpp>

namespace secureindex
{
    
    template <typename T>
    struct FriendMaker
    {
        typedef T Type;
    };

    template <class T>
    class Singleton : boost::noncopyable
    {
    public:

//    friend class FriendMaker<T>::Type;

        static T *instance()
        //static method that returns only instance of MySingletone
            {
                if (m_pOnlyOneInstance == 0) // if not yet instantiated
                {
                    m_pOnlyOneInstance = new T();
                    //create one and only object
                }
                return m_pOnlyOneInstance;
            }

        static T * getInstance()
            {
                T * m_instance = Singleton<T>::instance();
                return m_instance;
            }
        
    protected:

        static T * m_pOnlyOneInstance;
        //holds one and only object of MySingleton

        Singleton<T>(){}

    };

    template<class T>
    T* Singleton<T>::  m_pOnlyOneInstance = 0;
}
#endif
