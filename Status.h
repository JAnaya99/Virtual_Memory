#ifndef VIRTUAL_MEMORY_STATUS_H
#define VIRTUAL_MEMORY_STATUS_H

#include <string>

namespace virtual_memory {

    // There are two types of status OK and ERROR.
    enum class Status {
        kOk = 0,
        kError,
        kEnd
    };

    // This class is a better implementation of try - catch.
    // It allows you to call functions and know the status of the return value.
    template<class T>
    class StatusOr {
    public:
        //Constructors.
        StatusOr() = default;

        StatusOr(Status status, T data, std::string message);

        StatusOr(Status status, T data);

        StatusOr(Status status, std::string message);

        StatusOr(const StatusOr &other) = default;

        StatusOr &operator=(const StatusOr &other) = default;

        //Getters.
        Status GetStatus();

        std::string GetMessage();

        T GetData();

    private:
        Status status_;
        T data_;
        std::string message_;
    };

    template<class T>
    StatusOr<T>::StatusOr(Status status, T data, std::string message) : status_(status), data_(data),
                                                                        message_(message) {}

    template<class T>
    StatusOr<T>::StatusOr(Status status, T data) : status_(status), data_(data) {}

    template<class T>
    StatusOr<T>::StatusOr(Status status, std::string message) : status_(status), message_(message) {}

    template<class T>
    Status StatusOr<T>::GetStatus() {
        return status_;
    }

    template<class T>
    std::string StatusOr<T>::GetMessage() {
        return message_;
    }

    template<class T>
    T StatusOr<T>::GetData() {
        return data_;
    }

} // virtual_memory




#endif //VIRTUAL_MEMORY_STATUS_H