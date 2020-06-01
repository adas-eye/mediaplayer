#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex>
#include <memory>

template<typename T>
class Singleton {
public:

    //获取全局单例对象
    template<typename ...Args>
    static std::shared_ptr<T> GetInstance(Args&&... args) {
        if (!m_pSington) {
            std::lock_guard<std::mutex> gLock(m_Mutex);
            if (nullptr == m_pSington) {
                m_pSington = std::make_shared<T>(std::forward<Args>(args)...);
            }
        }
        return m_pSington;
    }

    //主动析构单例对象（一般不需要主动析构，除非特殊需求）
    static void DesInstance() {
        if (m_pSington) {
            m_pSington.reset();
            m_pSington = nullptr;
        }
    }

private:
    explicit Singleton();
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    ~Singleton();

private:
    static std::shared_ptr<T> m_pSington;
    static std::mutex m_Mutex;
};

template<typename T>
std::shared_ptr<T> Singleton<T>::m_pSington = nullptr;

template<typename T>
std::mutex Singleton<T>::m_Mutex;

#endif // SINGLETON_H
