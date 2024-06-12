#ifndef LQTUTILS_ENUM_H
#define LQTUTILS_ENUM_H

#include <QObject>
#include <QQmlEngine>

#define L_DECLARE_ENUM(enumName, ...)                                                      \
  namespace enumName {                                                                     \
  Q_NAMESPACE                                                                              \
  enum Value { __VA_ARGS__ };                                                              \
  Q_ENUM_NS(Value)                                                                         \
  inline int qmlRegister##enumName(const char *uri, int major, int minor)                  \
  {                                                                                        \
    return qmlRegisterUncreatableMetaObject(enumName::staticMetaObject, uri, major, minor, \
                                            #enumName, "Access to enums & flags only");    \
  }                                                                                        \
  inline int qRegisterMetaType()                                                           \
  {                                                                                        \
    return ::qRegisterMetaType<enumName::Value>(#enumName);                                \
  }                                                                                        \
  inline void registerEnum(const char *uri, int major, int minor)                          \
  {                                                                                        \
    enumName::qmlRegister##enumName(uri, major, minor);                                    \
    enumName::qRegisterMetaType();                                                         \
  }                                                                                        \
  }

#endif // LQTUTILS_ENUM_H
