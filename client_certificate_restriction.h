#pragma once

#include "envoy/network/connection.h"
#include "envoy/network/filter.h"

#include "common/common/logger.h"

namespace Envoy {
namespace Filter {

/**
 * A client SSL certificate restriction filter instance. One per connection.
 */
class ClientCertificateRestrictionFilter
    : public Network::ReadFilter,
      public Network::ConnectionCallbacks,
      Logger::Loggable<Logger::Id::filter> {
public:
  // Network::ReadFilter
  Network::FilterStatus onData(Buffer::Instance &data,
                               bool end_stream) override;
  Network::FilterStatus onNewConnection() override;
  void initializeReadFilterCallbacks(
      Network::ReadFilterCallbacks &callbacks) override {
    read_callbacks_ = &callbacks;
    read_callbacks_->connection().addConnectionCallbacks(*this);
  }

  // Network::ConnectionCallbacks
  void onEvent(Network::ConnectionEvent event) override;
  void onAboveWriteBufferHighWatermark() override {}
  void onBelowWriteBufferLowWatermark() override {}

private:
  Network::ReadFilterCallbacks *read_callbacks_{};
};

} // namespace Filter
} // namespace Envoy
