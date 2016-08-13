
#ifndef DIAG_INIT_H_
#define DIAG_INIT_H_

void is_DM_allowed(u_int32_t DMstate); /* Currently not used */
int  diag_com_write_command(int fd,u_int8_t deviceType,u_int16_t items[]);
int  diaginit_diag_com_write_command(u_int8_t deviceType,u_int16_t items[]);
int  diaginit_diag_com_write_event(u_int16_t Event_item[]);
int  diag_com_gps_write_command(int fd);
int  diag_com_gps_write_command(int fd);
int  Dm_cmd_time_request(int fd);
void diag_com_cdma_request(int fd,u_int16_t code);

#endif /* DIAG_INIT_H_ */
