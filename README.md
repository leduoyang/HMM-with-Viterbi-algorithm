#  HMM & Viterbi algo.

Training the discrete HMM model with training sequence and Testing the trained model with testing sequence by Viterbi algorithm

## Instruction for Executing
#### step 1.make file
#### step 2.Training
```
./train #_of_iteration initial_model training_data output_model_name

e.g.
./train 100 model_init.txt seq_model_01.txt model_01.txt
```

#### step 3.Testing after generating the models
```
./test model_list testing_data result_name

e.g.
./test modellist.txt testing_data1.txt result1.txt
```

## Example

### initial model
![alt text](https://github.com/leduoyang/DSP2017_HW/blob/master/hmm_viterbi/img/init_model.png)

### training data
![alt text](https://github.com/leduoyang/DSP2017_HW/blob/master/hmm_viterbi/img/training_data.png)

### trained model
![alt text](https://github.com/leduoyang/DSP2017_HW/blob/master/hmm_viterbi/img/trained_model.png)

### testing result
![alt text](https://github.com/leduoyang/DSP2017_HW/blob/master/hmm_viterbi/img/testing_result.png)
