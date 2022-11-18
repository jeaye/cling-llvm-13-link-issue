#pragma once

struct pool_item_common_base
{
  virtual ~pool_item_common_base() = default;
  virtual void inc_reference() = 0;
  virtual void release() = 0;
};

template <typename T>
struct pool_item_base : pool_item_common_base
{
  void inc_reference() override
  {}
  void release() override
  {}
};
