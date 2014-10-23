#define DATA_SIZE 10 // as a test, define 10 as the maximum number of data
#define NUM_OF_RANDOM 3 // small number to increase chance of number not changing

// the buffer that transfers data should be as big as the maximum number of data * 4 
// for the actual values to be transferred and their indices
// + 3 for 0xc0 and the bytes that goes into [repack]
const int total_size = (DATA_SIZE * 4) + 3;

byte data_buffer[total_size]; // set a maximum size for the array
byte stored_data[DATA_SIZE]; // array for value comparison

void setup() {
  Serial.begin(9600);
  // initialize values in array that stores data for comparison
  for(int i = 0; i < DATA_SIZE; i++)
    stored_data[i] = random(NUM_OF_RANDOM);
}

void loop() {
  int repack_size = 0;
  int final_size;
  data_buffer[0] = 0xc0;
  // give an offset of 3, cause during the process you calculate the size of data
  // that needs to be the second value in the list and it's split in two
  int index = 3;
  // set random values and check whether changed
  // if changed stored them in data_buffer and afterwards store its index
  for(int i = 0; i < DATA_SIZE; i++){
    byte val = random(NUM_OF_RANDOM);
    if(val != stored_data[i]){
      data_buffer[index++] = val & 0x007f;
      data_buffer[index++] = val >> 7;
      data_buffer[index++] = i & 0x007f;
      data_buffer[index++] = i >> 7;
      stored_data[i] = val;
      // increment the size of the data that will be transferred
      repack_size++;
    }
  }
  // quadraple repack_size cause bytes are being split in two
  // and we send a value index along with that value, also split in two
  repack_size *= 4;
  // second (and third) element in array is the number of values to transfer
  // excluding itself, sent to [repack]
  data_buffer[1] = repack_size & 0x007f;
  data_buffer[2] = repack_size >> 7;
  // set transfer bytes size
  // + 3 is for the number that goes into [repack] and 0xc0
  final_size = repack_size + 3;
  // in case any number has changed write it to the serial line
  if(repack_size) Serial.write(data_buffer, final_size);
  // wait for a second
  delay(1000);
}
