/*
 * cpp-periphery
 * https://github.com/mpb27/cpp-periphery
 * License: MIT
 *
 * Notes:
 *   1) Don't throw from destructor:
 *          https://stackoverflow.com/questions/130117
 *   2) RAII example :
 *          https://www.tomdalling.com/blog/software-design/resource-acquisition-is-initialisation-raii-explained/
 *   3) Deleteing copy constructor in C++11:
 *          https://stackoverflow.com/questions/33776697
 */

#ifndef PERIPHERY_I2C_HPP
#define PERIPHERY_I2C_HPP


#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <system_error>
#include <initializer_list>
#include <vector>

namespace periphery {

class I2C {
public:
    class Message;  // ... forward define ...

    // ... constructor / destructor ...
    I2C(const std::string& path);
    ~I2C();
    // ... disable copy-constructor and copy assignment ...
    I2C(const I2C&) = delete;
    I2C& operator=(const I2C&) = delete;

    // ... primary functions ...
    void transfer(uint16_t addr, Message& message) const;
    void transfer(uint16_t addr, std::initializer_list<std::reference_wrapper<Message>> messages) const;
    template <typename ForwardIt>
    void transfer(uint16_t addr, ForwardIt begin, ForwardIt end) const;
    template <class... Messages>
    void transfer(uint16_t addr, Messages&&... messages) const;

    // ... extra ...
    std::string toString() const;

    class Message {
    public:

        static Message write(std::initializer_list<uint8_t> tx_data);  // has to be int to detect bad values
        static Message write(const std::vector<uint8_t>& tx_data);
        static Message read(size_t len);

        std::vector<uint8_t> data;

    private:
        uint16_t flags;
        Message() { };
        friend class I2C;
    };
private:
    int fd_;
    std::string path_;
};

} // ... namespace periphery ...


#endif  // PERIPHERY_I2C_HPP