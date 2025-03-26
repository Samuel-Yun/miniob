#include "common/lang/comparator.h"
#include "common/lang/sstream.h"
#include "common/log/log.h"
#include "common/value.h"
#include "date_type.h"
#include <cstdio>
#include <sstream>
#include <cstdlib>

// 构造函数：指定类型为 DATE（需要在 AttrType 中定义）
DateType::DateType() : DataType(AttrType::DATE) {}

DateType::~DateType() {}

// 比较两个日期值：返回负值、0或正值
int DateType::compare(const Value &left, const Value &right) const {
  ASSERT(left.attr_type() == AttrType::DATE && right.attr_type() == AttrType::DATE, "invalid type");
  const Date *left_date = reinterpret_cast<const Date*>(left.value_.pointer_value_);
  const Date *right_date = reinterpret_cast<const Date*>(right.value_.pointer_value_);
  if (left_date->value < right_date->value) {
    return -1;
  } else if (left_date->value > right_date->value) {
    return 1;
  }
  return 0;
}

// 从字符串 "YYYY-MM-DD" 解析日期，并将其存入 Value 中
RC DateType::set_value_from_str(Value &val, const std::string &data) const {
  // 格式检查：长度必须为 10 且第5、第8个字符为 '-'
  if (data.length() != 10 || data[4] != '-' || data[7] != '-') {
    return RC::FAILURE;
  }
  int year, month, day;
  if (std::sscanf(data.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
    return RC::FAILURE;
  }
  // 月份检查
  if (month < 1 || month > 12) {
    return RC::FAILURE;
  }
  int days_in_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
  // 闰年判断：能被4整除且不能被100整除，或者能被400整除
  if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
    days_in_month[1] = 29;
  }
  if (day < 1 || day > days_in_month[month - 1]) {
    return RC::FAILURE;
  }
  // 转换为整数形式表示日期：YYYYMMDD
  int64_t date_value = static_cast<int64_t>(year) * 10000 + month * 100 + day;
  // 分配 Date 结构体内存并赋值（实际使用时注意内存管理）
  Date *date_ptr = new Date();
  date_ptr->value = date_value;
  // 使用友元方式直接访问 set_pointer 方法
  val.set_pointer(date_ptr);
  return RC::SUCCESS;
}

// 将内部日期值转换为字符串 "YYYY-MM-DD"
RC DateType::to_string(const Value &val, std::string &result) const {
  ASSERT(val.attr_type() == AttrType::DATE, "invalid type");
  const Date *date_ptr = reinterpret_cast<const Date*>(val.value_.pointer_value_);
  int date_value = static_cast<int>(date_ptr->value);
  int year  = date_value / 10000;
  int month = (date_value / 100) % 100;
  int day   = date_value % 100;
  char buffer[16];
  std::snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", year, month, day);
  result = buffer;
  return RC::SUCCESS;
}

// 如有需要，可实现其它数据类型的转换，目前未实现
RC DateType::cast_to(const Value &val, AttrType type, Value &result) const {
  return RC::UNIMPLEMENTED;
}

int DateType::cast_cost(AttrType type) {
  if (type == AttrType::DATE) {
    return 0;
  }
  return INT32_MAX;
}
