#pragma once

#include "common/sys/rc.h"
#include "common/type/data_type.h"
#include <string>

// 内部日期表示结构体，采用整数表示格式 YYYYMMDD
struct Date {
  int64_t value;
};

class DateType : public DataType {
public:
  DateType();
  virtual ~DateType();

  // 比较两个日期值
  int compare(const Value &left, const Value &right) const override;

  // 从字符串中解析日期，并存储到 Value 中
  RC set_value_from_str(Value &val, const std::string &data) const override;

  // 将内部日期值转换为字符串输出
  RC to_string(const Value &val, std::string &result) const override;

  // 如有需要，可以添加其他类型转换函数
  RC cast_to(const Value &val, AttrType type, Value &result) const override;
  int cast_cost(AttrType type) override;
};