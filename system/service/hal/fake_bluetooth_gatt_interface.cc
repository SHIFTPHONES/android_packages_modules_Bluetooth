//
//  Copyright (C) 2015 Google, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at:
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include "service/hal/fake_bluetooth_gatt_interface.h"

namespace bluetooth {
namespace hal {
namespace {

// The global test handler instances. We have to have globals since the HAL
// interface methods all have to be global and their signatures don't allow us
// to pass in user_data.
std::shared_ptr<FakeBluetoothGattInterface::TestAdvertiserHandler> g_advertiser_handler;
std::shared_ptr<FakeBluetoothGattInterface::TestClientHandler> g_client_handler;
std::shared_ptr<FakeBluetoothGattInterface::TestServerHandler> g_server_handler;

bt_status_t FakeRegisterClient(bt_uuid_t* app_uuid) {
  if (g_client_handler)
    return g_client_handler->RegisterClient(app_uuid);

  return BT_STATUS_FAIL;
}

bt_status_t FakeUnregisterClient(int client_if) {
  if (g_client_handler)
    return g_client_handler->UnregisterClient(client_if);

  return BT_STATUS_FAIL;
}

bt_status_t FakeScan(bool start) {
  if (g_client_handler)
    return g_client_handler->Scan(start);

  return BT_STATUS_FAIL;
}

bt_status_t FakeConnect(int client_if, const bt_bdaddr_t *bd_addr,
                        bool is_direct, int transport) {
  if (g_client_handler)
    return g_client_handler->Connect(client_if, bd_addr, is_direct, transport);

  return BT_STATUS_FAIL;
}

bt_status_t FakeDisconnect(int client_if, const bt_bdaddr_t *bd_addr,
                           int conn_id) {
  if (g_client_handler)
    return g_client_handler->Disconnect(client_if, bd_addr, conn_id);

  return BT_STATUS_FAIL;
}

bt_status_t FakeRegisterServer(bt_uuid_t* app_uuid) {
  if (g_server_handler)
    return g_server_handler->RegisterServer(app_uuid);

  return BT_STATUS_FAIL;
}

bt_status_t FakeUnregisterServer(int server_if) {
  if (g_server_handler)
    return g_server_handler->UnregisterServer(server_if);

  return BT_STATUS_FAIL;
}

bt_status_t FakeAddService(
    int server_if, vector<btgatt_db_element_t> service) {
  if (g_server_handler)
    return g_server_handler->AddService(server_if, std::move(service));

  return BT_STATUS_FAIL;
}

bt_status_t FakeDeleteService(int server_if, int srvc_handle) {
  if (g_server_handler)
    return g_server_handler->DeleteService(server_if, srvc_handle);

  return BT_STATUS_FAIL;
}

bt_status_t FakeSendIndication(int server_if, int attribute_handle,
                               int conn_id, int confirm,
                               vector<uint8_t> value) {
  if (g_server_handler)
    return g_server_handler->SendIndication(server_if, attribute_handle,
                                            conn_id, confirm, std::move(value));

  return BT_STATUS_FAIL;
}

bt_status_t FakeSendResponse(int conn_id, int trans_id, int status,
                             btgatt_response_t* response) {
  if (g_server_handler)
    return g_server_handler->SendResponse(conn_id, trans_id, status, response);

  return BT_STATUS_FAIL;
}

btgatt_client_interface_t fake_btgattc_iface = {
  FakeRegisterClient,
  FakeUnregisterClient,
  FakeScan,
  FakeConnect,
  FakeDisconnect,
  nullptr,  // listen
  nullptr,  // refresh
  nullptr,  // search_service
  nullptr,  // read_characteristic
  nullptr,  // write_characteristic
  nullptr,  // read_descriptor
  nullptr,  // write_descriptor
  nullptr,  // execute_write
  nullptr,  // register_for_notification
  nullptr,  // deregister_for_notification
  nullptr,  // read_remote_rssi
  nullptr,  // scan_filter_param_setup
  nullptr,  // scan_filter_add_remove
  nullptr,  // scan_filter_clear
  nullptr,  // scan_filter_enable
  nullptr,  // get_device_type
  nullptr,  // configure_mtu
  nullptr,  // conn_parameter_update
  nullptr,  // set_scan_parameters
  nullptr,  // batchscan_cfg_storate
  nullptr,  // batchscan_enb_batch_scan
  nullptr,  // batchscan_dis_batch_scan
  nullptr,  // batchscan_read_reports
  nullptr,  // test_command
  nullptr,  // get_gatt_db
};

bt_status_t FakeRegisterAdvertiser(bt_uuid_t* app_uuid) {
  if (g_advertiser_handler)
    return g_advertiser_handler->RegisterAdvertiser(app_uuid);

  return BT_STATUS_FAIL;
}

bt_status_t FakeUnregisterAdvertiser(int advertiser_id) {
  if (g_advertiser_handler)
    return g_advertiser_handler->UnregisterAdvertiser(advertiser_id);

  return BT_STATUS_FAIL;
}

bt_status_t FakeMultiAdvEnable(
    int advertiser_id, int min_interval, int max_interval, int adv_type,
    int chnl_map, int tx_power, int timeout_s) {
  if (g_advertiser_handler)
    return g_advertiser_handler->MultiAdvEnable(advertiser_id, min_interval, max_interval,
                                     adv_type, chnl_map, tx_power, timeout_s);

  return BT_STATUS_FAIL;
}

bt_status_t FakeMultiAdvSetInstData(
    int advertiser_id, bool set_scan_rsp, bool include_name, bool incl_txpower,
    int appearance, vector<uint8_t> manufacturer_data,
    vector<uint8_t> service_data,
    vector<uint8_t> service_uuid) {
  if (g_advertiser_handler)
    return g_advertiser_handler->MultiAdvSetInstData(
        advertiser_id, set_scan_rsp, include_name, incl_txpower, appearance,
        manufacturer_data, service_data, service_uuid);

  return BT_STATUS_FAIL;
}

bt_status_t FakeMultiAdvDisable(int advertiser_id) {
  if (g_advertiser_handler)
    return g_advertiser_handler->MultiAdvDisable(advertiser_id);

  return BT_STATUS_FAIL;
}

ble_advertiser_interface_t fake_bt_advertiser_iface = {
  FakeRegisterAdvertiser,
  FakeUnregisterAdvertiser,
  nullptr,  // set_adv_data
  FakeMultiAdvEnable,
  nullptr,  // multi_adv_update
  FakeMultiAdvSetInstData,
  FakeMultiAdvDisable,
};

btgatt_server_interface_t fake_btgatts_iface = {
  FakeRegisterServer,
  FakeUnregisterServer,
  nullptr,  // connect
  nullptr,  // disconnect
  FakeAddService,
  nullptr,  // stop_service
  FakeDeleteService,
  FakeSendIndication,
  FakeSendResponse,
};

}  // namespace

FakeBluetoothGattInterface::FakeBluetoothGattInterface(
    std::shared_ptr<TestAdvertiserHandler> advertiser_handler,
    std::shared_ptr<TestClientHandler> client_handler,
    std::shared_ptr<TestServerHandler> server_handler)
    : advertiser_handler_(advertiser_handler),
      client_handler_(client_handler) {
  CHECK(!g_advertiser_handler);
  CHECK(!g_client_handler);
  CHECK(!g_server_handler);

  // We allow passing NULL. In this case all calls we fail by default.
  if (advertiser_handler)
    g_advertiser_handler = advertiser_handler;

  if (client_handler)
    g_client_handler = client_handler;

  if (server_handler)
    g_server_handler = server_handler;
}

FakeBluetoothGattInterface::~FakeBluetoothGattInterface() {
  if (g_advertiser_handler)
    g_advertiser_handler = nullptr;

  if (g_client_handler)
    g_client_handler = nullptr;

  if (g_server_handler)
    g_server_handler = nullptr;
}

// The methods below can be used to notify observers with certain events and
// given parameters.
void FakeBluetoothGattInterface::NotifyRegisterClientCallback(
    int status, int client_if,
    const bt_uuid_t& app_uuid) {
  FOR_EACH_OBSERVER(ClientObserver, client_observers_,
                    RegisterClientCallback(this, status, client_if, app_uuid));
}

void FakeBluetoothGattInterface::NotifyConnectCallback(
    int conn_id, int status, int client_if, const bt_bdaddr_t& bda) {
  FOR_EACH_OBSERVER(ClientObserver, client_observers_,
                    ConnectCallback(this, conn_id, status, client_if, bda));
}

void FakeBluetoothGattInterface::NotifyDisconnectCallback(
    int conn_id, int status, int client_if, const bt_bdaddr_t& bda) {
  FOR_EACH_OBSERVER(ClientObserver, client_observers_,
                    DisconnectCallback(this, conn_id, status, client_if, bda));
}

void FakeBluetoothGattInterface::NotifyScanResultCallback(
    const bt_bdaddr_t& bda, int rssi, vector<uint8_t> adv_data) {
  FOR_EACH_OBSERVER(ClientObserver, client_observers_,
                    ScanResultCallback(this, bda, rssi, adv_data));
}

void FakeBluetoothGattInterface::NotifyRegisterAdvertiserCallback(
    int status, int advertiser_id,
    const bt_uuid_t& app_uuid) {
  FOR_EACH_OBSERVER(AdvertiserObserver, advertiser_observers_,
                    RegisterAdvertiserCallback(this, status, advertiser_id, app_uuid));
}

void FakeBluetoothGattInterface::NotifyMultiAdvEnableCallback(
    int advertiser_id, int status) {
  FOR_EACH_OBSERVER(AdvertiserObserver, advertiser_observers_,
                    MultiAdvEnableCallback(this, advertiser_id, status));
}

void FakeBluetoothGattInterface::NotifyMultiAdvDataCallback(
    int advertiser_id, int status) {
  FOR_EACH_OBSERVER(AdvertiserObserver, advertiser_observers_,
                    MultiAdvDataCallback(this, advertiser_id, status));
}

void FakeBluetoothGattInterface::NotifyMultiAdvDisableCallback(
    int advertiser_id, int status) {
  FOR_EACH_OBSERVER(AdvertiserObserver, advertiser_observers_,
                    MultiAdvDisableCallback(this, advertiser_id, status));
}

void FakeBluetoothGattInterface::NotifyRegisterServerCallback(
    int status, int server_if,
    const bt_uuid_t& app_uuid) {
  FOR_EACH_OBSERVER(ServerObserver, server_observers_,
                    RegisterServerCallback(this, status, server_if, app_uuid));
}

void FakeBluetoothGattInterface::NotifyServerConnectionCallback(
    int conn_id, int server_if, int connected, const bt_bdaddr_t& bda) {
  FOR_EACH_OBSERVER(
      ServerObserver, server_observers_,
      ConnectionCallback(this, conn_id, server_if, connected, bda));
}

void FakeBluetoothGattInterface::NotifyServiceAddedCallback(
    int status, int server_if,
    vector<btgatt_db_element_t> service) {
  FOR_EACH_OBSERVER(
      ServerObserver, server_observers_,
      ServiceAddedCallback(this, status, server_if, service));
}

void FakeBluetoothGattInterface::NotifyRequestReadCharacteristicCallback(
    int conn_id, int trans_id, const bt_bdaddr_t& bda, int attr_handle,
    int offset, bool is_long) {
  FOR_EACH_OBSERVER(
      ServerObserver, server_observers_,
      RequestReadCharacteristicCallback(
          this, conn_id, trans_id, bda, attr_handle, offset, is_long));
}

void FakeBluetoothGattInterface::NotifyRequestReadDescriptorCallback(
    int conn_id, int trans_id, const bt_bdaddr_t& bda, int attr_handle,
    int offset, bool is_long) {
  FOR_EACH_OBSERVER(
      ServerObserver, server_observers_,
      RequestReadDescriptorCallback(
          this, conn_id, trans_id, bda, attr_handle, offset, is_long));
}

void FakeBluetoothGattInterface::NotifyRequestWriteCharacteristicCallback(
    int conn_id, int trans_id, const bt_bdaddr_t& bda, int attr_handle,
    int offset, bool need_rsp, bool is_prep, vector<uint8_t> value) {
  FOR_EACH_OBSERVER(
      ServerObserver, server_observers_,
      RequestWriteCharacteristicCallback(
          this, conn_id, trans_id, bda, attr_handle, offset, need_rsp,
          is_prep, value));
}

void FakeBluetoothGattInterface::NotifyRequestWriteDescriptorCallback(
    int conn_id, int trans_id,
    const bt_bdaddr_t& bda, int attr_handle,
    int offset, bool need_rsp, bool is_prep,
    vector<uint8_t> value) {
  FOR_EACH_OBSERVER(
      ServerObserver, server_observers_,
      RequestWriteDescriptorCallback(
          this, conn_id, trans_id, bda, attr_handle, offset, need_rsp,
          is_prep, value));
}

void FakeBluetoothGattInterface::NotifyRequestExecWriteCallback(
    int conn_id, int trans_id, const bt_bdaddr_t& bda, int exec_write) {
  FOR_EACH_OBSERVER(
      ServerObserver, server_observers_,
      RequestExecWriteCallback(this, conn_id, trans_id, bda, exec_write));
}

void FakeBluetoothGattInterface::NotifyIndicationSentCallback(
    int conn_id, int status) {
  FOR_EACH_OBSERVER(ServerObserver, server_observers_,
                    IndicationSentCallback(this, conn_id, status));
}

void FakeBluetoothGattInterface::AddAdvertiserObserver(AdvertiserObserver* observer) {
  CHECK(observer);
  advertiser_observers_.AddObserver(observer);
}

void FakeBluetoothGattInterface::RemoveAdvertiserObserver(
    AdvertiserObserver* observer) {
  CHECK(observer);
  advertiser_observers_.RemoveObserver(observer);
}

void FakeBluetoothGattInterface::AddClientObserver(ClientObserver* observer) {
  CHECK(observer);
  client_observers_.AddObserver(observer);
}

void FakeBluetoothGattInterface::RemoveClientObserver(
    ClientObserver* observer) {
  CHECK(observer);
  client_observers_.RemoveObserver(observer);
}

void FakeBluetoothGattInterface::AddServerObserver(ServerObserver* observer) {
  CHECK(observer);
  server_observers_.AddObserver(observer);
}

void FakeBluetoothGattInterface::RemoveServerObserver(
    ServerObserver* observer) {
  CHECK(observer);
  server_observers_.RemoveObserver(observer);
}

const ble_advertiser_interface_t*
FakeBluetoothGattInterface::GetAdvertiserHALInterface() const {
  return &fake_bt_advertiser_iface;
}

const btgatt_client_interface_t*
FakeBluetoothGattInterface::GetClientHALInterface() const {
  return &fake_btgattc_iface;
}

const btgatt_server_interface_t*
FakeBluetoothGattInterface::GetServerHALInterface() const {
  return &fake_btgatts_iface;
}

}  // namespace hal
}  // namespace bluetooth
