#pragma once

#include "envoy/stats/stats.h"

#include "source/common/stats/metric_impl.h"

namespace Envoy {
namespace Stats {

/**
 * Null map implementation.
 * No-ops on all calls and requires no underlying metric or data.
 */
class NullMapImpl : public MetricImpl<Map> {
public:
  explicit NullMapImpl(SymbolTable& symbol_table)
      : MetricImpl<Map>(symbol_table), symbol_table_(symbol_table) {}
   ~NullMapImpl() override {
    // MetricImpl must be explicitly cleared() before destruction, otherwise it
    // will not be able to access the SymbolTable& to free the symbols. An RAII
    // alternative would be to store the SymbolTable reference in the
    // MetricImpl, costing 8 bytes per stat.
    MetricImpl::clear(symbol_table_);
  }

  void insert_request_sent(absl::string_view, absl::string_view, const Http::RequestHeaderMap*) override {}
  void insert_request_recvd(absl::string_view) override {}
  bool setHandled(absl::string_view) override { return {}; }
  bool insert_trace_recvd(absl::string_view, absl::string_view,
                          const Http::RequestHeaderMap*) { return {}; }
  const MsgHistory* getMsgHistory(absl::string_view) override { return {}; }

  // Metric
  bool used() const override { return false; }
  SymbolTable& symbolTable() override { return symbol_table_; }

  // RefcountInterface
  void incRefCount() override { refcount_helper_.incRefCount(); }
  bool decRefCount() override { return refcount_helper_.decRefCount(); }
  uint32_t use_count() const override { return refcount_helper_.use_count(); }

private:
  RefcountHelper refcount_helper_;
  SymbolTable& symbol_table_;
};

} // namespace Stats
} // namespace Envoy
