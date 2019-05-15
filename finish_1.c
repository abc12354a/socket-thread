//
// Created by angus on 19-4-20.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

struct send_byte {
    uint8_t b_head[2];
    uint8_t b_len[2];
    uint8_t b_ins[2];
    uint8_t b_data[0];
};

pthread_mutex_t mutex;
const uint8_t ins1[6] = {0x4e, 0x66, 0x00, 0x00, 0x43, 0x05};
const uint8_t hb[6] = {0x4e, 0x66, 0x00, 0x14, 0x72, 0x66};
const uint8_t version[6] = {0x4e, 0x66, 0x00, 0x41, 0x34, 0x02};
const uint8_t speed[6] = {0x4e, 0x66, 0x00, 0x00, 0x26, 0x02};
const uint8_t operation[6] = {0x4e, 0x66, 0x00, 0x00, 0x21, 0x02};
const uint8_t coord[6] = {0x4e, 0x66, 0x00, 0x00, 0x3c, 0x0b};
const uint8_t robottype[6] = {0x4e, 0x66, 0x00, 0x00, 0x2e, 0x02};
const uint8_t robitmode[6] = {0x4e, 0x66, 0x00, 0x00, 0x50, 0x02};
const uint8_t robotsum[6] = {0x4e, 0x66, 0x00, 0x00, 0x2e, 0x05};
const uint8_t servestatus[6] = {0x4e, 0x66, 0x00, 0x0c, 0x20, 0x02};
const uint8_t serveset[6] = {0x4e, 0x66, 0x00, 0x0d, 0x20, 0x01};
const uint8_t cordmode[6] = {0x4e, 0x66, 0x00, 0x0c, 0x22, 0x02};
const uint8_t globalvarint[6] = {0x4e, 0x66, 0x00, 0x0e, 0x56, 0x08};
const uint8_t serveip[6] = {0x4e, 0x66, 0x00, 0x00, 0x43, 0x02};
const uint8_t controllisence[6] = {0x4e, 0x66, 0x00, 0x00, 0x50, 0x52};
const uint8_t controlcord[6] = {0x4e, 0x66, 0x00, 0x00, 0x2e, 0x08};
const uint8_t press_stop[6] = {0x4e, 0x66, 0x00, 0x00, 0x29, 0x02};
const uint8_t press_sub[6] = {0x4e, 0x66, 0x00, 0x1a, 0x29, 0x01};
const uint8_t press_plus[6] = {0x4e, 0x66, 0x00, 0x19, 0x29, 0x01};
const uint8_t coord_switch[6] = {0x4e, 0x66, 0x00, 0x16, 0x22, 0x01};
const uint8_t speed_change1[6] = {0x4e, 0x66, 0x00, 0x0c, 0x26, 0x01};
const uint8_t speed_change2[6] = {0x4e, 0x66, 0x00, 0x0d, 0x26, 0x01};
const uint8_t speed_change3[6] = {0x4e, 0x66, 0x00, 0x0e, 0x26, 0x01};
const uint8_t serve_change[6] = {0x4e, 0x66, 0x00, 0x0e, 0x23, 0x01};
const uint8_t send_ins1[31] = {
        0x4e, 0x66, 0x00, 0x15, 0x43, 0x06, 0x7b, 0x22, 0x66, 0x69, 0x6e, 0x69, 0x73, 0x68, 0x69, 0x6e, 0x69, 0x74,
        0x22, 0x3a, 0x22, 0x74, 0x72, 0x75, 0x65, 0x22, 0x7d, 0xba, 0xc0, 0xaa, 0x6f};
const uint8_t send_version[40] = {
        0x4e, 0x66, 0x00, 0x41, 0x34, 0x03, 0x7b, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x22, 0x3a, 0x22,
        0x76, 0x31, 0x2e, 0x30, 0x2d, 0x72, 0x63, 0x31, 0x2d, 0x36, 0x37, 0x2d, 0x67, 0x66, 0x33, 0x34, 0x64, 0x61,
        0x65, 0x37, 0x22, 0x7d};
const uint8_t send_speed[31] = {
        0x4e, 0x66, 0x00, 0x15, 0x26, 0x03, 0x7b, 0x22, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x22, 0x3a, 0x31, 0x2c, 0x22,
        0x73, 0x70, 0x65, 0x65, 0x64, 0x22, 0x3a, 0x35, 0x7d, 0xF1, 0xE8, 0xDE, 0x73};
const uint8_t send_opreation[20] = {
        0x4e, 0x66, 0x00, 0x0a, 0x21, 0x03, 0x7b, 0x22, 0x6d, 0x6f, 0x64, 0x65, 0x22, 0x3a, 0x30, 0x7d, 0x75, 0xAE,
        0x3A, 0xAB};
const uint8_t send_serve[41] = {
        0x4e, 0x66, 0x00, 0x1f, 0x20, 0x03, 0x7b, 0x22, 0x6d, 0x6f, 0x64, 0x65, 0x22, 0x3a, 0x30, 0x2c, 0x22, 0x72,
        0x6f, 0x62, 0x6f, 0x74, 0x22, 0x3a, 0x31, 0x2c, 0x22, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x22, 0x3a, 0x33,
        0x7d, 0x29, 0xF0, 0x58, 0x10};
const uint8_t send_joint[31] = {
        0x4e, 0x66, 0x00, 0x15, 0x22, 0x03, 0x7b, 0x22, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x22, 0x3a, 0x31, 0x2c, 0x22,
        0x63, 0x6f, 0x6f, 0x72, 0x64, 0x22, 0x3a, 0x30, 0x7d, 0x3f, 0x3b, 0x87, 0x3e};
const uint8_t send_cart[31] = {
        0x4e, 0x66, 0x00, 0x15, 0x22, 0x03, 0x7b, 0x22, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x22, 0x3a, 0x31, 0x2c, 0x22,
        0x63, 0x6f, 0x6f, 0x72, 0x64, 0x22, 0x3a, 0x31, 0x7d, 0x26, 0x20, 0xb6, 0x7f};
const uint8_t send_tool[31] = {
        0x4e, 0x66, 0x00, 0x15, 0x22, 0x03, 0x7b, 0x22, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x22, 0x3a, 0x31, 0x2c, 0x22,
        0x63, 0x6f, 0x6f, 0x72, 0x64, 0x22, 0x3a, 0x32, 0x7d, 0x0d, 0x0d, 0xe5, 0xbc};
const uint8_t send_user[31] = {
        0x4e, 0x66, 0x00, 0x15, 0x22, 0x03, 0x7b, 0x22, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x22, 0x3a, 0x31, 0x2c, 0x22,
        0x63, 0x6f, 0x6f, 0x72, 0x64, 0x22, 0x3a, 0x33, 0x7d, 0x14, 0x16, 0xd4, 0xfd};
const uint8_t send_speed5[31] = {
        0x4e, 0x66, 0x00, 0x15, 0x26, 0x03, 0x7b, 0x22, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x22, 0x3a, 0x31, 0x2c, 0x22,
        0x73, 0x70, 0x65, 0x65, 0x64, 0x22, 0x3a, 0x35, 0x7d, 0xf1, 0xe8, 0xde, 0x73};
const uint8_t send_speed1[31] = {
        0x4e, 0x66, 0x00, 0x15, 0x26, 0x03, 0x7b, 0x22, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x22, 0x3a, 0x31, 0x2c, 0x22,
        0x73, 0x70, 0x65, 0x65, 0x64, 0x22, 0x3a, 0x31, 0x7d, 0x95, 0x84, 0x1b, 0x77};
const uint8_t send_speed15[32] = {
        0x4e, 0x66, 0x00, 0x16, 0x26, 0x03, 0x7b, 0x22, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x22, 0x3a, 0x31, 0x2c, 0x22,
        0x73, 0x70, 0x65, 0x65, 0x64, 0x22, 0x3a, 0x31, 0x35, 0x7d, 0x3f, 0x0f, 0xf1, 0xd6};
const uint8_t send_speed25[32] = {
        0x4e, 0x66, 0x00, 0x16, 0x26, 0x03, 0x7b, 0x22, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x22, 0x3a, 0x31, 0x2c, 0x22,
        0x73, 0x70, 0x65, 0x65, 0x64, 0x22, 0x3a, 0x32, 0x35, 0x7d, 0x3d, 0x49, 0x4f, 0x8f};
const uint8_t send_speed75[32] = {
        0x4e, 0x66, 0x00, 0x16, 0x26, 0x03, 0x7b, 0x22, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x22, 0x3a, 0x31, 0x2c, 0x22,
        0x73, 0x70, 0x65, 0x65, 0x64, 0x22, 0x3a, 0x37, 0x35, 0x7d, 0x3b, 0x82, 0x8d, 0x64};
const uint8_t send_speed50[32] = {
        0x4e, 0x66, 0x00, 0x16, 0x26, 0x03, 0x7b, 0x22, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x22, 0x3a, 0x31, 0x2c, 0x22,
        0x73, 0x70, 0x65, 0x65, 0x64, 0x22, 0x3a, 0x35, 0x30, 0x7d, 0x45, 0x71, 0xad, 0x4f};
const uint8_t send_speed90[32] = {
        0x4e, 0x66, 0x00, 0x16, 0x26, 0x03, 0x7b, 0x22, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x22, 0x3a, 0x31, 0x2c, 0x22,
        0x73, 0x70, 0x65, 0x65, 0x64, 0x22, 0x3a, 0x39, 0x30, 0x7d, 0x4c, 0x6b, 0x54, 0x2b};
const uint8_t send_speed100[33] = {
        0x4e, 0x66, 0x00, 0x17, 0x26, 0x03, 0x7b, 0x22, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x22, 0x3a, 0x31, 0x2c, 0x22,
        0x73, 0x70, 0x65, 0x65, 0x64, 0x22, 0x3a, 0x31, 0x30, 0x30, 0x7d, 0x37, 0x38, 0xcf, 0x28};
const uint8_t send_speed101[33] = {
        0x4e, 0x66, 0x00, 0x17, 0x26, 0x03, 0x7b, 0x22, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x22, 0x3a, 0x31, 0x2c, 0x22,
        0x73, 0x70, 0x65, 0x65, 0x64, 0x22, 0x3a, 0x31, 0x30, 0x31, 0x7d, 0x2e, 0x23, 0xfe, 0x69};
const uint8_t send_speed102[33] = {
        0x4e, 0x66, 0x00, 0x17, 0x26, 0x03, 0x7b, 0x22, 0x72, 0x6f, 0x62, 0x6f, 0x74, 0x22, 0x3a, 0x31, 0x2c, 0x22,
        0x73, 0x70, 0x65, 0x65, 0x64, 0x22, 0x3a, 0x31, 0x30, 0x32, 0x7d, 0x05, 0x0e, 0xad, 0xaa};
const uint8_t send_status_off[24] = {
        0x4e, 0x66, 0x00, 0x0e, 0x23, 0x03, 0x7b, 0x22, 0x64, 0x65, 0x61, 0x64, 0x6d, 0x61, 0x6e, 0x22, 0x3a, 0x30,
        0x7d, 0x64, 0xd4, 0xc5, 0x55};
const uint8_t send_status_on[24] = {
        0x4e, 0x66, 0x00, 0x0e, 0x23, 0x03, 0x7b, 0x22, 0x64, 0x65, 0x61, 0x64, 0x6d, 0x61, 0x6e, 0x22, 0x3a, 0x31,
        0x7d, 0x65, 0x16, 0xaf, 0x62};
static uint32_t table[256];
int switch_press = 1;

//位逆转
static uint32_t bitrev(uint32_t input, int bw) {
    int i;
    uint32_t var;
    var = 0;
    for (i = 0; i < bw; i++) {
        if (input & 0x01) {
            var |= 1 << (bw - 1 - i);
        }
        input >>= 1;
    }
    return var;
}


//码表生成
void crc32_init(uint32_t poly) {
    int i;
    int j;
    uint32_t c;


    poly = bitrev(poly, 32);
    for (i = 0; i < 256; i++) {
        c = i;
        for (j = 0; j < 8; j++) {
            c = (c & 1) ? (poly ^ (c >> 1)) : (c >> 1);
        }
        table[i] = c;
    }
}


//计算CRC
uint32_t crc32(uint32_t crc, void *input, int len) {
    int i;
    uint8_t index;
    uint8_t *p;
    p = (uint8_t *) input;
    for (i = 0; i < len; i++) {
        index = (*p ^ crc);
        crc = (crc >> 8) ^ table[index];
        p++;
    }
    return crc;
}

void *read_msg(void *fd) {
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    int count4 = 0;
    int count5 = 0;
    int count6 = 0;
    int count7 = 0;
    int count8 = 0;
    int count9 = 0;
    int count10 = 0;
    int count11 = 0;
    int count12 = 0;
    int sockfd = *(int *) fd;
    pthread_mutex_unlock(&mutex);
    printf("thread_read-msg\n");
    char buff_send[] = "control";
    char buff_send2[] = "heart";
    char no_msg[] = "no msg";
    uint8_t waitcrc[24] = {0};
    uint8_t str[80];
    uint8_t send_data[] = {0x7b, 0x22, 0x66, 0x69, 0x6e, 0x69, 0x73, 0x68, 0x69, 0x6e, 0x69, 0x74, 0x22, 0x3a, 0x22,
                           0x74, 0x72, 0x75, 0x65, 0x22, 0x7d, 0xba, 0xc0, 0xaa, 0x6f};
    struct send_byte *send_buffer = NULL;
    send_buffer = (struct send_byte *) malloc(sizeof(struct send_byte) + sizeof(uint8_t) * 24);
    send_buffer->b_head[0] = 0x4e;
    send_buffer->b_head[1] = 0x66;
    send_buffer->b_len[0] = 0x00;
    send_buffer->b_len[1] = 0x14;
    send_buffer->b_ins[0] = 0x72;
    send_buffer->b_ins[1] = 0x67;
    while (1) {
        memset(str, 64, 80);
        int r_len = read(sockfd, str, sizeof(str) - 1);
        if (r_len <= 0)
            break;
        //printf("Message form server:\n");
        for (int i = 0; i < r_len; ++i) {
            if (str[i] == ins1[i] && count1 <= 6)
                count1++;
            if (str[i] == hb[i] && count2 <= 6)
                count2++;
            if (str[i] == version[i] && count3 <= 6)
                count3++;
            if (str[i] == serveset[i] && count4 <= 6)
                count4++;
            if (str[i] == press_stop[i] && count5 <= 6)
                count5++;
            if (str[i] == press_plus[i] && count6 <= 6)
                count6++;
            if (str[i] == press_sub[i] && count7 <= 6)
                count7++;
            if (str[i] == coord_switch[i] && count8 <= 6)
                count8++;
            if (str[i] == speed_change1[i] && count9 <= 6)
                count9++;
            if (str[i] == speed_change2[i] && count10 <= 6)
                count10++;
            if (str[i] == speed_change3[i] && count11 <= 6)
                count11++;
            if (str[i] == serve_change[i] && count12 <= 6)
                count12++;
            printf("%02hhx", (uint8_t) str[i]);
        }
        printf("\n");
        if (count1 == 6) {
            printf("control\n");
            write(sockfd, send_ins1, sizeof(send_ins1));
        }
        else if (count2 == 6) {
            printf("heart\n");
            for (int i = 0; i < 24; i++) {
                waitcrc[i] = str[i + 2];
            }
            waitcrc[3] = 0x67;
            uint32_t crc;
            crc32_init(0x4C11DB7);
            crc = (crc32((uint32_t) 0xFFFFFFFF, waitcrc, 24) ^ 0xFFFFFFFF);
            uint8_t crc_8[4] = {0xFF, 0xFF, 0xFF, 0xFF};
            crc_8[0] &= crc;
            crc_8[1] &= crc >> 8;
            crc_8[2] &= crc >> 16;
            crc_8[3] &= crc >> 24;
            for (int j = 0; j < 20; ++j) {
                send_buffer->b_data[j] = waitcrc[j + 4];
            }
            for (int k = 0; k < 4; ++k) {
                send_buffer->b_data[k + 20] = crc_8[3 - k];
            }
            write(sockfd, send_buffer, 30 * sizeof(uint8_t));
        }
        else if (count3 == 6) {
            printf("version\n");
            write(sockfd, send_version, sizeof(send_version));
        }
        else if (count4 == 6) {
            printf("set serve\n");
            write(sockfd, send_serve, sizeof(send_serve));
        }
        else if (count5 == 6) {
            printf("stop press\n");
        }
        else if (count6 == 6) {
            printf("%d no msg6\n", count7);
            switch (str[34]) {
                case (uint8_t) 0x33:
                    if(switch_press == 1)
                        printf("S+ preesed\n");
                    else if (switch_press == 2)
                        printf("X+ pressed\n");
                    else if (switch_press == 3)
                        printf("TX+ pressed\n");
                    else
                        printf("UX+ pressed\n");
                    break;
                case (uint8_t) 0xb7:
                    if(switch_press == 1)
                        printf("L+ preesed\n");
                    else if (switch_press == 2)
                        printf("Y+ pressed\n");
                    else if (switch_press == 3)
                        printf("TY+ pressed\n");
                    else
                        printf("UY+ pressed\n");
                    break;
                case (uint8_t) 0xf4:
                    if(switch_press == 1)
                        printf("U+ preesed\n");
                    else if (switch_press == 2)
                        printf("Z+ pressed\n");
                    else if (switch_press == 3)
                        printf("TZ+ pressed\n");
                    else
                        printf("UZ+ pressed\n");
                    break;
                case (uint8_t) 0xbf:
                    if(switch_press == 1)
                        printf("R+ preesed\n");
                    else if (switch_press == 2)
                        printf("A+ pressed\n");
                    else if (switch_press == 3)
                        printf("TA+ pressed\n");
                    else
                        printf("UA+ pressed\n");
                    break;
                case (uint8_t) 0xfc:
                    if(switch_press == 1)
                        printf("B+ preesed\n");
                    else if (switch_press == 2)
                        printf("B+ pressed\n");
                    else if (switch_press == 3)
                        printf("TB+ pressed\n");
                    else
                        printf("UB+ pressed\n");
                    break;
                case (uint8_t) 0x78:
                    if(switch_press == 1)
                        printf("T+ preesed\n");
                    else if (switch_press == 2)
                        printf("C+ pressed\n");
                    else if (switch_press == 3)
                        printf("TC+ pressed\n");
                    else
                        printf("UC+ pressed\n");
                    break;
                default:
                    break;
            }
        }
        else if (count7 == 7) {
            printf("test cout7\n");
            switch (str[35]) {
                case (uint8_t) 0x76:
                    printf("test\n");
                    if(switch_press == 1)
                        printf("S- preesed\n");
                    else if (switch_press == 2)
                        printf("X- pressed\n");
                    else if (switch_press == 3)
                        printf("TX- pressed\n");
                    else
                        printf("UX- pressed\n");
                    break;
                case (uint8_t) 0x14:
                    if(switch_press == 1)
                        printf("L- preesed\n");
                    else if (switch_press == 2)
                        printf("Y- pressed\n");
                    else if (switch_press == 3)
                        printf("TY- pressed\n");
                    else
                        printf("UY- pressed\n");
                    break;
                case (uint8_t) 0xf5:
                    if(switch_press == 1)
                        printf("U- preesed\n");
                    else if (switch_press == 2)
                        printf("Z- pressed\n");
                    else if (switch_press == 3)
                        printf("TZ- pressed\n");
                    else
                        printf("UZ- pressed\n");
                    break;
                case (uint8_t) 0x91:
                    if(switch_press == 1)
                        printf("R- preesed\n");
                    else if (switch_press == 2)
                        printf("A- pressed\n");
                    else if (switch_press == 3)
                        printf("TA- pressed\n");
                    else
                        printf("UA- pressed\n");
                    break;
                case (uint8_t) 0x70:
                    if(switch_press == 1)
                        printf("B- preesed\n");
                    else if (switch_press == 2)
                        printf("B- pressed\n");
                    else if (switch_press == 3)
                        printf("TB- pressed\n");
                    else
                        printf("UB- pressed\n");
                    break;
                case (uint8_t) 0x12:
                    if(switch_press == 1)
                        printf("T- preesed\n");
                    else if (switch_press == 2)
                        printf("C- pressed\n");
                    else if (switch_press == 3)
                        printf("TC- pressed\n");
                    else
                        printf("UC- pressed\n");
                    break;
                default:
                    break;
            }
        }
        else if (count8 == 6) {
            printf("%d no msg8\n", count7);
            switch (switch_press) {
                case 0:
                    printf("change to joint\n");
                    write(sockfd, send_joint, sizeof(send_joint));
                    switch_press = 1;
                    break;
                case 1:
                    printf("change to cart\n");
                    write(sockfd, send_cart, sizeof(send_cart));
                    switch_press = 2;
                    break;
                case 2:
                    printf("change to tool\n");
                    write(sockfd, send_tool, sizeof(send_tool));
                    switch_press = 3;
                    break;
                case 3:
                    printf("change to user\n");
                    write(sockfd, send_user, sizeof(send_user));
                    switch_press = 0;
                    break;
                default:
                    switch_press = 0;
                    break;
            }
        }
        else if (count9 == 6) {
            switch (str[21]) {
                case (uint8_t) 0x56:
                    printf("change speed to 5\n");
                    write(sockfd, send_speed5, sizeof(send_speed5));
                    break;
                case (uint8_t) 0x8a:
                    printf("change speed to 1\n");
                    write(sockfd, send_speed1, sizeof(send_speed1));
                    break;
                default:
                    break;
            }
        }
        else if (count10 == 6) {
            switch (str[22]) {
                case (uint8_t) 0x16:
                    printf("change speed to 15\n");
                    write(sockfd, send_speed15, sizeof(send_speed15));
                    break;
                case (uint8_t) 0xf8:
                    printf("change speed to 25\n");
                    write(sockfd, send_speed25, sizeof(send_speed25));
                    break;
                case (uint8_t) 0xaa:
                    printf("change speed to 50\n");
                    write(sockfd, send_speed50, sizeof(send_speed50));
                    break;
                case (uint8_t) 0xca:
                    printf("change speed to 75\n");
                    write(sockfd, send_speed75, sizeof(send_speed75));
                    break;
                case (uint8_t) 0x12:
                    printf("change speed to 90\n");
                    write(sockfd, send_speed90, sizeof(send_speed90));
                    break;
                default:
                    break;
            }

        } else if (count11 == 6) {
            switch (str[23]) {
                case (uint8_t) 0x09:
                    printf("change speed to 100\n");
                    write(sockfd, send_speed100, sizeof(send_speed100));
                    break;
                case (uint8_t) 0x67:
                    printf("change speed to 0.01\n");
                    write(sockfd, send_speed102, sizeof(send_speed102));
                    break;
                case (uint8_t) 0x3e:
                    printf("change speed to 0.1\n");
                    write(sockfd, send_speed101, sizeof(send_speed101));
                    break;
                default:
                    break;
            }
        } else if (count12 == 6) {
            printf("test\n");
            switch (str[23]) {
                case (uint8_t) 0xf3:
                    printf("status: on\n");
                    write(sockfd, send_status_on, sizeof(send_status_on));
                    break;
                case (uint8_t) 0xc4:
                    printf("status: off\n");
                    write(sockfd, send_status_off, sizeof(send_status_off));
                    break;
                default:
                    break;
            }

        } else {
            printf("%d no msg\n", count7);
            count7 = 0;
            //write(sockfd, no_msg, sizeof(no_msg));
        }
        count1 = 0;
        count2 = 0;
        count3 = 0;
        count4 = 0;
        count5 = 0;
        count6 = 0;
        count7 = 0;
        count8 = 0;
        count9 = 0;
        count10 = 0;
        count11 = 0;
        count12 = 0;
    }
    printf("close connetion\n");
    close(sockfd);
}

//void *send_msg(void *fd) {
//    struct send_byte {
//        uint8_t b_head[2];
//        uint8_t b_len[2];
//        uint8_t b_ins[2];
//        uint8_t b_data[0];
//    };
//    uint8_t send_data[] = {0x7b, 0x22, 0x66, 0x69, 0x6e, 0x69, 0x73, 0x68, 0x69, 0x6e, 0x69, 0x74, 0x22, 0x3a, 0x22,
//                           0x74, 0x72, 0x75, 0x65, 0x22, 0x7d, 0xba, 0xc0, 0xaa, 0x6f};
//    struct send_byte *send_buffer = NULL;
//    send_buffer = (struct send_byte *) malloc(sizeof(struct send_byte) + sizeof(uint8_t) * 25);
//    send_buffer->b_head[0] = 0x4e;
//    send_buffer->b_head[1] = 0x66;
//    send_buffer->b_len[0] = 0x00;
//    send_buffer->b_len[1] = 0x15;
//    send_buffer->b_ins[0] = 0x43;
//    send_buffer->b_ins[1] = 0x06;
//    for (int i = 0; i < 25; ++i) {
//        send_buffer->b_data[i] = send_data[i];
//    }
//    int sockfd = *(int *) fd;
//    printf("thread_send-msg\n");
//    while (1) {
//        write(sockfd, send_buffer, 31 * sizeof(uint8_t));
//        sleep(5);
//    }//while
//    //printf("\n");
//}

int main() {
    pthread_mutex_init(&mutex, NULL);
    //创建套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    pthread_t read_id, send_id;
    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("192.168.1.13");  //具体的IP地址
    serv_addr.sin_port = htons(6000);  //端口
    bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    //进入监听状态，等待用户发起请求
    listen(serv_sock, 20);
    while (1) {
        //接收客户端请求
        struct sockaddr_in clnt_addr;
        uint8_t str[80];
        memset(str, 64, 80);
        socklen_t clnt_addr_size = sizeof(clnt_addr);
        pthread_mutex_lock(&mutex);
        int clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_addr_size);
        printf("ready to pthread\n");
        if ((pthread_create(&read_id, NULL, read_msg, &clnt_sock)) == -1) {
            perror("pthread create error");
            exit(1);
        }
        pthread_detach(read_id);
    }
//    while (1) {
//        write(clnt_sock, send_buffer, 31 * sizeof(uint8_t));
//        sleep(5);
//    }//while
//    int r_len = read(clnt_sock, str, sizeof(str) - 1);
//    printf("Message form server: %s\n",str);
//    for (int i = 0; i < r_len; ++i) {
//        if (str[i] != 64)
//            //printf("%c",str[i]);
//            printf("%02hhx", (unsigned char) str[i]);
//    }
//    printf("\n");
//        for (int i = 0; i < sizeof(str); ++i) {
//           printf("%x\n", str[i]);
//        }
    //关闭套接字
    close(serv_sock);
    //printf("exit main func\n");
    return 0;
}
