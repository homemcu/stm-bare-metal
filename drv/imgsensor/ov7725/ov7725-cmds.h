#ifndef OV7725_CMDS_H_
#define OV7725_CMDS_H_

// All this stuff has been taken from the Linux driver
// https://github.com/torvalds/linux/blob/master/drivers/media/i2c/ov772x.c

#define GAIN        0x00 /* AGC - Gain control gain setting */
#define BLUE        0x01 /* AWB - Blue channel gain setting */
#define RED         0x02 /* AWB - Red   channel gain setting */
#define GREEN       0x03 /* AWB - Green channel gain setting */
#define COM1        0x04 /* Common control 1 */
#define BAVG        0x05 /* U/B Average Level */
#define GAVG        0x06 /* Y/Gb Average Level */
#define RAVG        0x07 /* V/R Average Level */
#define AECH        0x08 /* Exposure Value - AEC MSBs */
#define COM2        0x09 /* Common control 2 */
#define PID         0x0A /* Product ID Number MSB */
#define VER         0x0B /* Product ID Number LSB */
#define COM3        0x0C /* Common control 3 */
#define COM4        0x0D /* Common control 4 */
#define COM5        0x0E /* Common control 5 */
#define COM6        0x0F /* Common control 6 */
#define AEC         0x10 /* Exposure Value */
#define CLKRC       0x11 /* Internal clock */
#define COM7        0x12 /* Common control 7 */
#define COM8        0x13 /* Common control 8 */
#define COM9        0x14 /* Common control 9 */
#define COM10       0x15 /* Common control 10 */
#define REG16       0x16 /* Register 16 */
#define HSTART      0x17 /* Horizontal sensor size */
#define HSIZE       0x18 /* Horizontal frame (HREF column) end high 8-bit */
#define VSTART      0x19 /* Vertical frame (row) start high 8-bit */
#define VSIZE       0x1A /* Vertical sensor size */
#define PSHFT       0x1B /* Data format - pixel delay select */
#define MIDH        0x1C /* Manufacturer ID byte - high */
#define MIDL        0x1D /* Manufacturer ID byte - low  */
#define LAEC        0x1F /* Fine AEC value */
#define COM11       0x20 /* Common control 11 */
#define BDBASE      0x22 /* Banding filter Minimum AEC value */
#define DBSTEP      0x23 /* Banding filter Maximum Setp */
#define AEW         0x24 /* AGC/AEC - Stable operating region (upper limit) */
#define AEB         0x25 /* AGC/AEC - Stable operating region (lower limit) */
#define VPT         0x26 /* AGC/AEC Fast mode operating region */
#define REG28       0x28 /* Register 28 */
#define HOUTSIZE    0x29 /* Horizontal data output size MSBs */
#define EXHCH       0x2A /* Dummy pixel insert MSB */
#define EXHCL       0x2B /* Dummy pixel insert LSB */
#define VOUTSIZE    0x2C /* Vertical data output size MSBs */
#define ADVFL       0x2D /* LSB of insert dummy lines in Vertical direction */
#define ADVFH       0x2E /* MSG of insert dummy lines in Vertical direction */
#define YAVE        0x2F /* Y/G Channel Average value */
#define LUMHTH      0x30 /* Histogram AEC/AGC Luminance high level threshold */
#define LUMLTH      0x31 /* Histogram AEC/AGC Luminance low  level threshold */
#define HREF        0x32 /* Image start and size control */
#define DM_LNL      0x33 /* Dummy line low  8 bits */
#define DM_LNH      0x34 /* Dummy line high 8 bits */
#define ADOFF_B     0x35 /* AD offset compensation value for B  channel */
#define ADOFF_R     0x36 /* AD offset compensation value for R  channel */
#define ADOFF_GB    0x37 /* AD offset compensation value for Gb channel */
#define ADOFF_GR    0x38 /* AD offset compensation value for Gr channel */
#define OFF_B       0x39 /* Analog process B  channel offset value */
#define OFF_R       0x3A /* Analog process R  channel offset value */
#define OFF_GB      0x3B /* Analog process Gb channel offset value */
#define OFF_GR      0x3C /* Analog process Gr channel offset value */
#define COM12       0x3D /* Common control 12 */
#define COM13       0x3E /* Common control 13 */
#define COM14       0x3F /* Common control 14 */
#define COM15       0x40 /* Common control 15*/
#define COM16       0x41 /* Common control 16 */
#define TGT_B       0x42 /* BLC blue channel target value */
#define TGT_R       0x43 /* BLC red  channel target value */
#define TGT_GB      0x44 /* BLC Gb   channel target value */
#define TGT_GR      0x45 /* BLC Gr   channel target value */
/* for ov7720 */
#define LCC0        0x46 /* Lens correction control 0 */
#define LCC1        0x47 /* Lens correction option 1 - X coordinate */
#define LCC2        0x48 /* Lens correction option 2 - Y coordinate */
#define LCC3        0x49 /* Lens correction option 3 */
#define LCC4        0x4A /* Lens correction option 4 - radius of the circular */
#define LCC5        0x4B /* Lens correction option 5 */
#define LCC6        0x4C /* Lens correction option 6 */
/* for ov7725 */
#define LC_CTR      0x46 /* Lens correction control */
#define LC_XC       0x47 /* X coordinate of lens correction center relative */
#define LC_YC       0x48 /* Y coordinate of lens correction center relative */
#define LC_COEF     0x49 /* Lens correction coefficient */
#define LC_RADI     0x4A /* Lens correction radius */
#define LC_COEFB    0x4B /* Lens B channel compensation coefficient */
#define LC_COEFR    0x4C /* Lens R channel compensation coefficient */

#define FIXGAIN     0x4D /* Analog fix gain amplifer */
#define AREF0       0x4E /* Sensor reference control */
#define AREF1       0x4F /* Sensor reference current control */
#define AREF2       0x50 /* Analog reference control */
#define AREF3       0x51 /* ADC    reference control */
#define AREF4       0x52 /* ADC    reference control */
#define AREF5       0x53 /* ADC    reference control */
#define AREF6       0x54 /* Analog reference control */
#define AREF7       0x55 /* Analog reference control */
#define UFIX        0x60 /* U channel fixed value output */
#define VFIX        0x61 /* V channel fixed value output */
#define AWBB_BLK    0x62 /* AWB option for advanced AWB */
#define AWB_CTRL0   0x63 /* AWB control byte 0 */
#define DSP_CTRL1   0x64 /* DSP control byte 1 */
#define DSP_CTRL2   0x65 /* DSP control byte 2 */
#define DSP_CTRL3   0x66 /* DSP control byte 3 */
#define DSP_CTRL4   0x67 /* DSP control byte 4 */
#define AWB_BIAS    0x68 /* AWB BLC level clip */
#define AWB_CTRL1   0x69 /* AWB control  1 */
#define AWB_CTRL2   0x6A /* AWB control  2 */
#define AWB_CTRL3   0x6B /* AWB control  3 */
#define AWB_CTRL4   0x6C /* AWB control  4 */
#define AWB_CTRL5   0x6D /* AWB control  5 */
#define AWB_CTRL6   0x6E /* AWB control  6 */
#define AWB_CTRL7   0x6F /* AWB control  7 */
#define AWB_CTRL8   0x70 /* AWB control  8 */
#define AWB_CTRL9   0x71 /* AWB control  9 */
#define AWB_CTRL10  0x72 /* AWB control 10 */
#define AWB_CTRL11  0x73 /* AWB control 11 */
#define AWB_CTRL12  0x74 /* AWB control 12 */
#define AWB_CTRL13  0x75 /* AWB control 13 */
#define AWB_CTRL14  0x76 /* AWB control 14 */
#define AWB_CTRL15  0x77 /* AWB control 15 */
#define AWB_CTRL16  0x78 /* AWB control 16 */
#define AWB_CTRL17  0x79 /* AWB control 17 */
#define AWB_CTRL18  0x7A /* AWB control 18 */
#define AWB_CTRL19  0x7B /* AWB control 19 */
#define AWB_CTRL20  0x7C /* AWB control 20 */
#define AWB_CTRL21  0x7D /* AWB control 21 */
#define GAM1        0x7E /* Gamma Curve  1st segment input end point */
#define GAM2        0x7F /* Gamma Curve  2nd segment input end point */
#define GAM3        0x80 /* Gamma Curve  3rd segment input end point */
#define GAM4        0x81 /* Gamma Curve  4th segment input end point */
#define GAM5        0x82 /* Gamma Curve  5th segment input end point */
#define GAM6        0x83 /* Gamma Curve  6th segment input end point */
#define GAM7        0x84 /* Gamma Curve  7th segment input end point */
#define GAM8        0x85 /* Gamma Curve  8th segment input end point */
#define GAM9        0x86 /* Gamma Curve  9th segment input end point */
#define GAM10       0x87 /* Gamma Curve 10th segment input end point */
#define GAM11       0x88 /* Gamma Curve 11th segment input end point */
#define GAM12       0x89 /* Gamma Curve 12th segment input end point */
#define GAM13       0x8A /* Gamma Curve 13th segment input end point */
#define GAM14       0x8B /* Gamma Curve 14th segment input end point */
#define GAM15       0x8C /* Gamma Curve 15th segment input end point */
#define SLOP        0x8D /* Gamma curve highest segment slope */
#define DNSTH       0x8E /* De-noise threshold */
#define EDGE_STRNGT 0x8F /* Edge strength  control when manual mode */
#define EDGE_TRSHLD 0x90 /* Edge threshold control when manual mode */
#define DNSOFF      0x91 /* Auto De-noise threshold control */
#define EDGE_UPPER  0x92 /* Edge strength upper limit when Auto mode */
#define EDGE_LOWER  0x93 /* Edge strength lower limit when Auto mode */
#define MTX1        0x94 /* Matrix coefficient 1 */
#define MTX2        0x95 /* Matrix coefficient 2 */
#define MTX3        0x96 /* Matrix coefficient 3 */
#define MTX4        0x97 /* Matrix coefficient 4 */
#define MTX5        0x98 /* Matrix coefficient 5 */
#define MTX6        0x99 /* Matrix coefficient 6 */
#define MTX_CTRL    0x9A /* Matrix control */
#define BRIGHT      0x9B /* Brightness control */
#define CNTRST      0x9C /* Contrast contrast */
#define CNTRST_CTRL 0x9D /* Contrast contrast center */
#define UVAD_J0     0x9E /* Auto UV adjust contrast 0 */
#define UVAD_J1     0x9F /* Auto UV adjust contrast 1 */
#define SCAL0       0xA0 /* Scaling control 0 */
#define SCAL1       0xA1 /* Scaling control 1 */
#define SCAL2       0xA2 /* Scaling control 2 */
#define FIFODLYM    0xA3 /* FIFO manual mode delay control */
#define FIFODLYA    0xA4 /* FIFO auto   mode delay control */
#define SDE         0xA6 /* Special digital effect control */
#define USAT        0xA7 /* U component saturation control */
#define VSAT        0xA8 /* V component saturation control */
/* for ov7720 */
#define HUE0        0xA9 /* Hue control 0 */
#define HUE1        0xAA /* Hue control 1 */
/* for ov7725 */
#define HUECOS      0xA9 /* Cosine value */
#define HUESIN      0xAA /* Sine value */

#define SIGN        0xAB /* Sign bit for Hue and contrast */
#define DSPAUTO     0xAC /* DSP auto function ON/OFF control */

/*
 * register detail
 */

/* COM2 */
#define SOFT_SLEEP_MODE 0x10	/* Soft sleep mode */
				/* Output drive capability */
#define OCAP_1x         0x00	/* 1x */
#define OCAP_2x         0x01	/* 2x */
#define OCAP_3x         0x02	/* 3x */
#define OCAP_4x         0x03	/* 4x */

/* COM3 */
#define SWAP_MASK       (SWAP_RGB | SWAP_YUV | SWAP_ML)
#define IMG_MASK        (VFLIP_IMG | HFLIP_IMG)

#define VFLIP_IMG       0x80	/* Vertical flip image ON/OFF selection */
#define HFLIP_IMG       0x40	/* Horizontal mirror image ON/OFF selection */
#define SWAP_RGB        0x20	/* Swap B/R  output sequence in RGB mode */
#define SWAP_YUV        0x10	/* Swap Y/UV output sequence in YUV mode */
#define SWAP_ML         0x08	/* Swap output MSB/LSB */
				/* Tri-state option for output clock */
#define NOTRI_CLOCK     0x04	/*   0: Tri-state    at this period */
				/*   1: No tri-state at this period */
				/* Tri-state option for output data */
#define NOTRI_DATA      0x02	/*   0: Tri-state    at this period */
				/*   1: No tri-state at this period */
#define SCOLOR_TEST     0x01	/* Sensor color bar test pattern */

/* COM4 */
				/* PLL frequency control */
#define PLL_BYPASS      0x00	/*  00: Bypass PLL */
#define PLL_4x          0x40	/*  01: PLL 4x */
#define PLL_6x          0x80	/*  10: PLL 6x */
#define PLL_8x          0xc0	/*  11: PLL 8x */
				/* AEC evaluate window */
#define AEC_FULL        0x00	/*  00: Full window */
#define AEC_1p2         0x10	/*  01: 1/2  window */
#define AEC_1p4         0x20	/*  10: 1/4  window */
#define AEC_2p3         0x30	/*  11: Low 2/3 window */
#define COM4_RESERVED   0x01	/* Reserved bit */

/* COM5 */
#define AFR_ON_OFF      0x80	/* Auto frame rate control ON/OFF selection */
#define AFR_SPPED       0x40	/* Auto frame rate control speed selection */
				/* Auto frame rate max rate control */
#define AFR_NO_RATE     0x00	/*     No  reduction of frame rate */
#define AFR_1p2         0x10	/*     Max reduction to 1/2 frame rate */
#define AFR_1p4         0x20	/*     Max reduction to 1/4 frame rate */
#define AFR_1p8         0x30	/* Max reduction to 1/8 frame rate */
				/* Auto frame rate active point control */
#define AF_2x           0x00	/*     Add frame when AGC reaches  2x gain */
#define AF_4x           0x04	/*     Add frame when AGC reaches  4x gain */
#define AF_8x           0x08	/*     Add frame when AGC reaches  8x gain */
#define AF_16x          0x0c	/* Add frame when AGC reaches 16x gain */
				/* AEC max step control */
#define AEC_NO_LIMIT    0x01	/*   0 : AEC incease step has limit */
				/*   1 : No limit to AEC increase step */
/* CLKRC */
				/* Input clock divider register */
#define CLKRC_RESERVED  0x80	/* Reserved bit */
#define CLKRC_DIV(n)    ((n) - 1)

/* COM7 */
				/* SCCB Register Reset */
#define SCCB_RESET      0x80	/*   0 : No change */
				/*   1 : Resets all registers to default */
				/* Resolution selection */
#define SLCT_MASK       0x40	/*   Mask of VGA or QVGA */
#define SLCT_VGA        0x00	/*   0 : VGA */
#define SLCT_QVGA       0x40	/*   1 : QVGA */
#define ITU656_ON_OFF   0x20	/* ITU656 protocol ON/OFF selection */
#define SENSOR_RAW	0x10	/* Sensor RAW */
				/* RGB output format control */
#define FMT_MASK        0x0c	/*      Mask of color format */
#define FMT_GBR422      0x00	/*      00 : GBR 4:2:2 */
#define FMT_RGB565      0x04	/*      01 : RGB 565 */
#define FMT_RGB555      0x08	/*      10 : RGB 555 */
#define FMT_RGB444      0x0c	/* 11 : RGB 444 */
				/* Output format control */
#define OFMT_MASK       0x03    /*      Mask of output format */
#define OFMT_YUV        0x00	/*      00 : YUV */
#define OFMT_P_BRAW     0x01	/*      01 : Processed Bayer RAW */
#define OFMT_RGB        0x02	/*      10 : RGB */
#define OFMT_BRAW       0x03	/* 11 : Bayer RAW */

/* COM8 */
#define FAST_ALGO       0x80	/* Enable fast AGC/AEC algorithm */
				/* AEC Setp size limit */
#define UNLMT_STEP      0x40	/*   0 : Step size is limited */
				/*   1 : Unlimited step size */
#define BNDF_ON_OFF     0x20	/* Banding filter ON/OFF */
#define AEC_BND         0x10	/* Enable AEC below banding value */
#define AEC_ON_OFF      0x08	/* Fine AEC ON/OFF control */
#define AGC_ON          0x04	/* AGC Enable */
#define AWB_ON          0x02	/* AWB Enable */
#define AEC_ON          0x01	/* AEC Enable */

/* COM9 */
#define BASE_AECAGC     0x80	/* Histogram or average based AEC/AGC */
				/* Automatic gain ceiling - maximum AGC value */
#define GAIN_2x         0x00	/*    000 :   2x */
#define GAIN_4x         0x10	/*    001 :   4x */
#define GAIN_8x         0x20	/*    010 :   8x */
#define GAIN_16x        0x30	/*    011 :  16x */
#define GAIN_32x        0x40	/*    100 :  32x */
#define GAIN_64x        0x50	/* 101 :  64x */
#define GAIN_128x       0x60	/* 110 : 128x */
#define DROP_VSYNC      0x04	/* Drop VSYNC output of corrupt frame */
#define DROP_HREF       0x02	/* Drop HREF  output of corrupt frame */

/* COM11 */
#define SGLF_ON_OFF     0x02	/* Single frame ON/OFF selection */
#define SGLF_TRIG       0x01	/* Single frame transfer trigger */

/* HREF */
#define HREF_VSTART_SHIFT	6	/* VSTART LSB */
#define HREF_HSTART_SHIFT	4	/* HSTART 2 LSBs */
#define HREF_VSIZE_SHIFT	2	/* VSIZE LSB */
#define HREF_HSIZE_SHIFT	0	/* HSIZE 2 LSBs */

/* EXHCH */
#define EXHCH_VSIZE_SHIFT	2	/* VOUTSIZE LSB */
#define EXHCH_HSIZE_SHIFT	0	/* HOUTSIZE 2 LSBs */

/* DSP_CTRL1 */
#define FIFO_ON         0x80	/* FIFO enable/disable selection */
#define UV_ON_OFF       0x40	/* UV adjust function ON/OFF selection */
#define YUV444_2_422    0x20	/* YUV444 to 422 UV channel option selection */
#define CLR_MTRX_ON_OFF 0x10	/* Color matrix ON/OFF selection */
#define INTPLT_ON_OFF   0x08	/* Interpolation ON/OFF selection */
#define GMM_ON_OFF      0x04	/* Gamma function ON/OFF selection */
#define AUTO_BLK_ON_OFF 0x02	/* Black defect auto correction ON/OFF */
#define AUTO_WHT_ON_OFF 0x01	/* White define auto correction ON/OFF */

/* DSP_CTRL3 */
#define UV_MASK         0x80	/* UV output sequence option */
#define UV_ON           0x80	/*   ON */
#define UV_OFF          0x00	/*   OFF */
#define CBAR_MASK       0x20	/* DSP Color bar mask */
#define CBAR_ON         0x20	/*   ON */
#define CBAR_OFF        0x00	/*   OFF */

/* DSP_CTRL4 */
#define DSP_OFMT_YUV	0x00
#define DSP_OFMT_RGB	0x00
#define DSP_OFMT_RAW8	0x02
#define DSP_OFMT_RAW10	0x03

/* DSPAUTO (DSP Auto Function ON/OFF Control) */
#define AWB_ACTRL       0x80 /* AWB auto threshold control */
#define DENOISE_ACTRL   0x40 /* De-noise auto threshold control */
#define EDGE_ACTRL      0x20 /* Edge enhancement auto strength control */
#define UV_ACTRL        0x10 /* UV adjust auto slope control */
#define SCAL0_ACTRL     0x08 /* Auto scaling factor control */
#define SCAL1_2_ACTRL   0x04 /* Auto scaling factor control */

#endif // OV7725_CMDS_H_
