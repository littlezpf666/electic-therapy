#include "ili_lcd_general.h"
#include "lcd_api.h"

// Compatible list:
// ili9320 ili9325 ili9328
// LG4531
// SSD1289

//������������,�����������
#ifdef __CC_ARM                			 /* ARM Compiler 	*/
#define lcd_inline   				static __inline
#elif defined (__ICCARM__)        		/* for IAR Compiler */
#define lcd_inline 					inline
#elif defined (__GNUC__)        		/* GNU GCC Compiler */
#define lcd_inline 					static __inline
#else
#define lcd_inline                  static
#endif

/********* control <ֻ��ֲ���º�������> ***********/
#include "stm32f10x.h"

#include "usart2.h"

//����ض���.���������ض���ʱ,ʹ��stdio.h�е�printf���.
//#define printf               rt_kprintf //ʹ��rt_kprintf�����
//#define printf(...)                       //�����

static void delay(int cnt)
{
    volatile unsigned int dl;
    while(cnt--)
    {
        for(dl=0; dl<500; dl++);
    }
}

/*Ӳ����صĺ궨��*/
#define set_CS()        GPIOD->BSRR  = 0x00001000;
#define clr_CS()        GPIOD->BRR   = 0x00001000;

#define set_RS()        GPIOD->BSRR  = 0x00002000;
#define clr_RS()        GPIOD->BRR   = 0x00002000;

#define set_nWR()       GPIOD->BSRR  = 0x00004000;
#define clr_nWR()       GPIOD->BRR   = 0x00004000;

#define set_nRD()       GPIOD->BSRR  = 0x00008000;
#define clr_nRD()       GPIOD->BRR   = 0x00008000;

static void lcd_port_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*������Ӧʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|
                           RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);
    /*����Lcd��������Ϊ�������*/
    /*16λ����*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    /*���ƽ�*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    set_CS();
    set_RS();
    set_nRD();
    set_nWR();
}

lcd_inline void write_cmd(unsigned short cmd)
{
    /**********************************
    // ** nCS      ---\________/------*
    // ** RS       ----\______/-------*
    // ** nRD      -------------------*
    // ** nWR      -----\___/---------*
    // ** DB[0-15] ------[###]--------*
    **********************************/

    clr_CS();

    clr_RS();
    GPIOE->ODR = cmd;
    clr_nWR();
    set_nWR();
    set_RS();

    set_CS();
}

lcd_inline unsigned short read_data(void)
{
    /**********************************
    // ** nCS      ---\________/-----**
    // ** RS       ------------------**
    // ** nRD      -----\___/--------**
    // ** nWR      ------------------**
    // ** DB[0-15] ------[###]-------**
    **********************************/
    unsigned short val = 0;

    clr_CS();

    //��IO����Ϊ����ģʽ
    GPIOE->CRH = 0x44444444; // mode[3:2] => 01 => ��������ģʽ
    GPIOE->CRL = 0x44444444; // mode[1:0] => 00 => ����ģʽ

    clr_nRD();
    val = GPIOE->IDR;
    val = GPIOE->IDR;//������Ϊ�˱���,��Ϊ����������ӳ�.
    set_nRD();

    //��ȡ��Ϻ��ٴ�����Ϊ���ģʽ
    GPIOE->CRH = 0x33333333; // mode[3:2] => ͨ���������ģʽ
    GPIOE->CRL = 0x33333333; // mode[1:0] => ����ٶ�50M

    set_CS();
    return val;
}

void rw_data_prepare(void)
{
    write_cmd(0x22);
}

void write_data(unsigned short data )
{
    /**********************************
    // ** nCS      ---\________/-----**
    // ** RS       ------------------**
    // ** nRD      ------------------**
    // ** nWR      -----\___/--------**
    // ** DB[0-15] ------[###]-------**
    **********************************/
    clr_CS();

    GPIOE->ODR = data;
    clr_nWR();
    set_nWR();

    set_CS();
}

lcd_inline void write_reg(unsigned char reg_addr,unsigned short reg_val)
{
    /*************************************************
    // ** nCS      ---\________/------\________/----**
    // ** RS       ----\______/---------------------**
    // ** nRD      ---------------------------------**
    // ** nWR      -----\___/-----------\___/-------**
    // ** DB[0-15] ------[###]-----------[###]------**
    *************************************************/
    write_cmd(reg_addr);
    write_data(reg_val);
}

lcd_inline unsigned short read_reg(unsigned char reg_addr)
{
    /*************************************************
    // ** nCS      ---\________/------\________/----**
    // ** RS       ----\______/---------------------**
    // ** nRD      ---------------------\___/-------**
    // ** nWR      -----\___/-----------------------**
    // ** DB[0-15] ------[###]-----------[###]------**
    *************************************************/
    unsigned short tmp;
    write_cmd(reg_addr);
    tmp = read_data();
    return tmp;
}
/********* control <ֻ��ֲ���Ϻ�������> ***********/

static unsigned short deviceid=0;//����һ����̬������������LCD��ID

//����LCD��ID
unsigned int lcd_getdeviceid(void)
{
    return deviceid;
}

unsigned short BGR2RGB(unsigned short c)
{
    u16  r, g, b, rgb;

    b = (c>>0)  & 0x1f;
    g = (c>>5)  & 0x3f;
    r = (c>>11) & 0x1f;

    rgb =  (b<<11) + (g<<5) + (r<<0);

    return( rgb );
}

void lcd_SetCursor(unsigned int x,unsigned int y)
{
    // SSD1289 ����������ʹ�ò�ͬ�ļĴ���
    if( deviceid == 0x8999 )
    {
        write_reg(0x004e,x);    /* 0-239 */
			  if(y%2==0)
        write_reg(0x004f,y+1);    /* 0-319 */
				else
				write_reg(0x004f,y-1);    /* 0-319 */
    }
}

/* ��ȡָ����ַ��GRAM */
unsigned short lcd_read_gram(unsigned int x,unsigned int y)
{
    unsigned short temp;
    lcd_SetCursor(x,y);
    rw_data_prepare();
    /* dummy read */
    temp = read_data();
	  temp = read_data();
    return temp;
}

void lcd_clear(unsigned short Color)
{
    unsigned int index=0;
    lcd_SetCursor(0,0);
    rw_data_prepare();                      /* Prepare to write GRAM */
    for (index=0; index<(LCD_WIDTH*LCD_HEIGHT); index++)
    {
        write_data(Color);
    }
}

void lcd_data_bus_test(void)
{
    unsigned short temp1;
    unsigned short temp2;

    if(deviceid == 0x8999)
    {
        /* [5:4]-ID~ID0 [3]-AM-1��ֱ-0ˮƽ */
        write_reg(0x0011,0x6030 | (0<<3)); // AM=0 hline
    }

    /* wirte */
    lcd_SetCursor(0,0);
    rw_data_prepare();
    write_data(0x5555);
    write_data(0xAAAA);

    /* read */
    lcd_SetCursor(0,0);
    if( deviceid == 0x8999 )
    {
        temp1 = lcd_read_gram(0,0);
        temp2 = lcd_read_gram(1,0);
    }

    if( (temp1 == 0x5555) && (temp2 == 0xAAAA) )
    {
        printf(" data bus test pass!");
    }
    else
    {
        printf(" data bus test error: %04X %04X",temp1,temp2);
    }
}

void lcd_gram_test(void)
{
    unsigned short temp;
    unsigned int test_x;
    unsigned int test_y;

    printf(" LCD GRAM test....");
    /* write */
    temp=0;
    lcd_SetCursor(0,0);
    rw_data_prepare();
    for(test_y=0; test_y<76800; test_y++)
    {
        write_data(temp);
        temp++;
    }

    /* read */
    temp=0;

    if( deviceid == 0x8999 )
    {
        for(test_y=0; test_y<320; test_y++)
        {
            for(test_x=0; test_x<240; test_x++)
            {
                if(  lcd_read_gram(test_x,test_y) != temp++)
                {
                    printf("  LCD GRAM ERR!!");
                    return;
                }
            }
        }
        printf("  TEST PASS!\r\n");
    }
}

void lcd_Initializtion(void)
{
	  unsigned short deviceid_top; 
    lcd_port_init();
    delay(1500);
    deviceid = read_reg(0x00);
    /* deviceid check */
    if(deviceid != 0x8999)
    {
        printf("Invalid LCD ID:%08X\r\n",deviceid);
        printf("Please check you hardware and configure.");
        return;
    }
    else
    {
        printf("\r\nLCD Device ID : %04X ",deviceid);
    }

    if( deviceid == 0x8999 )
    {
        // power supply setting
        // set R07h at 0021h (GON=1,DTE=0,D[1:0]=01)
        write_reg(0x0007,0x0021);
        // set R00h at 0001h (OSCEN=1)
        write_reg(0x0000,0x0001);
        // set R07h at 0023h (GON=1,DTE=0,D[1:0]=11)
        write_reg(0x0007,0x0023);
        // set R10h at 0000h (Exit sleep mode)
        write_reg(0x0010,0x0000);
        // Wait 30ms
        delay(3000);
        // set R07h at 0033h (GON=1,DTE=1,D[1:0]=11)
        write_reg(0x0007,0x0033);
        // Entry mode setting (R11h)
        // R11H Entry mode
        // vsmode DFM1 DFM0 TRANS OEDef WMode DMode1 DMode0 TY1 TY0 ID1 ID0 AM LG2 LG2 LG0
        //   0     1    1     0     0     0     0      0     0   1   1   1  *   0   0   0
        write_reg(0x0011,0x6030);
        // LCD driver AC setting (R02h)
        write_reg(0x0002,0x0600);
        // power control 1
        // DCT3 DCT2 DCT1 DCT0 BT2 BT1 BT0 0 DC3 DC2 DC1 DC0 AP2 AP1 AP0 0
        // 1     0    1    0    1   0   0  0  1   0   1   0   0   1   0  0
        // DCT[3:0] fosc/4 BT[2:0]  DC{3:0] fosc/4
        write_reg(0x0003,0x0804);//0xA8A4
        write_reg(0x000C,0x0000);//
        write_reg(0x000D,0x080C);//
        // power control 4
        // 0 0 VCOMG VDV4 VDV3 VDV2 VDV1 VDV0 0 0 0 0 0 0 0 0
        // 0 0   1    0    1    0    1    1   0 0 0 0 0 0 0 0
        write_reg(0x000E,0x2900);
        write_reg(0x001E,0x00B8);
        write_reg(0x0001,0x2B3F);//�����������320*240  0x6B3F
        write_reg(0x0010,0x0000);
        write_reg(0x0005,0x0000);
        write_reg(0x0006,0x0000);
        write_reg(0x0016,0xEF1C);
        write_reg(0x0017,0x0003);
        write_reg(0x0007,0x0233);//0x0233
        write_reg(0x000B,0x0000|(3<<6));
        write_reg(0x000F,0x0000);//ɨ�迪ʼ��ַ
        write_reg(0x0041,0x0000);
        write_reg(0x0042,0x0000);
        write_reg(0x0048,0x0000);
        write_reg(0x0049,0x013F);
        write_reg(0x004A,0x0000);
        write_reg(0x004B,0x0000);
        write_reg(0x0044,0xEF00);
        write_reg(0x0045,0x0000);
        write_reg(0x0046,0x013F);
        write_reg(0x0030,0x0707);
        write_reg(0x0031,0x0204);
        write_reg(0x0032,0x0204);
        write_reg(0x0033,0x0502);
        write_reg(0x0034,0x0507);
        write_reg(0x0035,0x0204);
        write_reg(0x0036,0x0204);
        write_reg(0x0037,0x0502);
        write_reg(0x003A,0x0302);
        write_reg(0x003B,0x0302);
        write_reg(0x0023,0x0000);
        write_reg(0x0024,0x0000);
        write_reg(0x0025,0x8000);   // 65hz
    }

    //�������߲���,���ڲ���Ӳ�������Ƿ�����.
//    lcd_data_bus_test();
    //GRAM����,�˲��Կ��Բ���LCD�������ڲ�GRAM.����ͨ����֤Ӳ������
//    lcd_gram_test();
    //����
    lcd_clear( White );
}

//RTGUI�����ʵ��,����ʹ�� use_rt_gui ʱ,�Զ�����.
#if defined(use_rt_gui)
void rt_hw_lcd_update(rtgui_rect_t *rect)
{
    /* nothing for none-DMA mode driver */
}

rt_uint8_t * rt_hw_lcd_get_framebuffer(void)
{
    return RT_NULL; /* no framebuffer driver */
}

/*  �������ص� ��ɫ,X,Y */
void rt_hw_lcd_set_pixel(rtgui_color_t *c, rt_base_t x, rt_base_t y)
{
    unsigned short p;

    /* get color pixel */
    p = rtgui_color_to_565p(*c);
    lcd_SetCursor(x,y);

    rw_data_prepare();
    write_data(p);
}

/* ��ȡ���ص���ɫ */
void rt_hw_lcd_get_pixel(rtgui_color_t *c, rt_base_t x, rt_base_t y)
{
    unsigned short p;
    p = BGR2RGB( lcd_read_gram(x,y) );
    *c = rtgui_color_from_565p(p);
}

/* ��ˮƽ�� */
void rt_hw_lcd_draw_hline(rtgui_color_t *c, rt_base_t x1, rt_base_t x2, rt_base_t y)
{
    unsigned short p;

    /* get color pixel */
    p = rtgui_color_to_565p(*c);

    /* [5:4]-ID~ID0 [3]-AM-1��ֱ-0ˮƽ */
    write_reg(0x0003,(1<<12)|(1<<5)|(1<<4) | (0<<3) );

    lcd_SetCursor(x1, y);
    rw_data_prepare(); /* Prepare to write GRAM */
    while (x1 < x2)
    {
        write_data(p);
        x1++;
    }
}

/* ��ֱ�� */
void rt_hw_lcd_draw_vline(rtgui_color_t *c, rt_base_t x, rt_base_t y1, rt_base_t y2)
{
    unsigned short p;

    /* get color pixel */
    p = rtgui_color_to_565p(*c);

    /* [5:4]-ID~ID0 [3]-AM-1��ֱ-0ˮƽ */
    write_reg(0x0003,(1<<12)|(1<<5)|(0<<4) | (1<<3) );

    lcd_SetCursor(x, y1);
    rw_data_prepare(); /* Prepare to write GRAM */
    while (y1 < y2)
    {
        write_data(p);
        y1++;
    }
}

/* ?? */
void rt_hw_lcd_draw_raw_hline(rt_uint8_t *pixels, rt_base_t x1, rt_base_t x2, rt_base_t y)
{
    rt_uint16_t *ptr;

    /* get pixel */
    ptr = (rt_uint16_t*) pixels;

    /* [5:4]-ID~ID0 [3]-AM-1��ֱ-0ˮƽ */
    write_reg(0x0003,(1<<12)|(1<<5)|(1<<4) | (0<<3) );

    lcd_SetCursor(x1, y);
    rw_data_prepare(); /* Prepare to write GRAM */
    while (x1 < x2)
    {
        write_data(*ptr);
        x1 ++;
        ptr ++;
    }
}
#endif

