#include <stdio.h>
#include <stdint.h>


uint16_t VirtAddVarTab[255] = {0,1,2,3,4,254};

// 0 1 2 3 4 ; key 1  //
// 5 6 7 8 9 ; key 2

// 20 21 22 23 24 ; key 5

//key(n)  ((n-1)*5)+0  ((n-1)*5)+1  ((n-1)*5)+2 ((n-1)*5)+3 ((n-1)*5)+4 

int key[3][5] = {
					{0xFF, FF, FF, 9, 3}, 
					{13, 4, 8, 14, 1}, 
					{9, 6, 3, 7, 21}
				};


uint16_t new_id = 1;


#define NUMBER_OF_CARD  10
#define ID_OF_CARD  5


read_id()  // return 12,23,34,45,56 = 


uint8_t* id = read_id();

uint8_t check_card(uint8_t* id,  uint8_t key_id)
{
      
     uint8_t ret = 0;

    uint8_t key_tem[5];

    for(uint8_t i = 0; i < 5; i++ )
    {
    	key_tem[i] = key[key_id][i];
    }

    if(MFRC522_Compare(id, key_tem) == 1) 
		ret = 1;

    return ret;
}



uint8_t MFRC522_Compare(uint8_t* CardID, uint8_t* CompareID) {
	uint8_t i;
	for (i = 0; i < 5; i++) {
		if (CardID[i] != CompareID[i]) return MI_ERR;
	}
	return MI_OK;
}


uint8_t MFRC522_Compare2(uint8_t* CardID, uint8_t  key_id) {
	uint8_t i;

	uint8_t CompareID[5];

    for(uint8_t i = 0; i < 5; i++ )
    {
    	CompareID[i] = key[key_id][i];
    }

	for (i = 0; i < 5; i++) {
		if (CardID[i] != CompareID[i]) return MI_ERR;
	}
	return MI_OK;
}




int main()
{
	EE_Init();
    
    // readback flash //
    key[0][0] = EE_ReadVariable(0);
    key[0][1] = EE_ReadVariable(1);
    key[0][2] = EE_ReadVariable(2);
    key[0][3] = EE_ReadVariable(3);
    key[0][4] = EE_ReadVariable(4);

    // cách 2 dingf for'
     for(uint8_t n = 0; n <3; n++ )
     {
     	 for(uint8_t j = 0; j <5; j++ )
     		key[n][j] = EE_ReadVariable(((n-1)*5)+j, 0xff);
    	//key[n][1] = EE_ReadVariable(((n-1)*5)+1);
    	//key[n][2] = EE_ReadVariable(((n-1)*5)+2);
    	//key[n][3] = EE_ReadVariable(((n-1)*5)+3);
    	//key[n][4] = EE_ReadVariable(((n-1)*5)+4);
     }

      new_id =  EE_ReadVariable(255);


      check_card(0) == 1;
      check_card(1) == 1;

 	  check_card(2) == 1;

}

uint8_t write_key(u)
{
	uint8_t* id = read_id();

	new_id++;
	(if (new_id > 10)
	{
		id =1;
	})

    EE_WriteVariable(((new_id-1)*5)+0, id[0] );
    EE_WriteVariable(((new_id-1)*5)+1, id[1] );
    EE_WriteVariable(((new_id-1)*5)+2, id[2] );
    EE_WriteVariable(((new_id-1)*5)+3, id[3] );
    EE_WriteVariable(((new_id-1)*5)+4, id[4] );

    
    EE_WriteVariable(255, new_id);
}

xoa thẻ()
{
	case xóa:
	{
		for(n=0; n < 4; n++)
		{
			//if(MFRC522_Compare2(CardID, n) == MI_OK) 
//
				//lcd_print();
				EE_WriteVariable(((n)*5)+0, 0xff );
    			EE_WriteVariable(((n)*5)+1, 0xff );
    			EE_WriteVariable(((n)*5)+2, 0xff );
    			EE_WriteVariable(((n)*5)+3, 0xff );
    			EE_WriteVariable(((n)*5)+4, 0xff );
    		}
    	}
	}

}