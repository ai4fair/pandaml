from keras.models import Model, Sequential
from keras.layers import Input, Dense, Dropout, BatchNormalization
from keras.layers import LSTM, Bidirectional, RepeatVector, TimeDistributed


# Hit_Pairs (Adeel)
def init_model_dnn(shape=None):
    """Dense Network (Functional API)"""
    inputs = Input(shape=(shape,))
    h = Dense(units=400, activation='tanh')(inputs)
    h = BatchNormalization()(h)  # BN works best after acitvation is applied.
    h = Dropout(0.5)(h)
    h = Dense(units=300, activation='tanh')(h) 
    h = Dropout(0.5)(h)
    h = Dense(units=200, activation='tanh')(h) 
    h = Dropout(0.5)(h)
    h = Dense(units=100, activation='tanh')(h) 
    h = Dropout(0.5)(h)
    h = Dense(50, activation='tanh')(h)
    outputs = Dense(1, activation='sigmoid')(h)
    model = Model(inputs=inputs, outputs=outputs)
    model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
    return model

# Hit_Pairs (Waleed)
def init_model(features=None):
    """Dense Network (Sequential API)"""
    model = Sequential()
    model.add(Dense(400, activation='tanh', input_shape=(features,), kernel_initializer='glorot_uniform'))
    model.add(Dropout(0.5))
    model.add(Dense(300, activation='tanh', kernel_initializer='glorot_uniform'))
    model.add(Dropout(0.5))
    model.add(Dense(200, activation='tanh', kernel_initializer='glorot_uniform'))
    model.add(Dropout(0.5))
    model.add(Dense(100, activation='tanh', kernel_initializer='glorot_uniform'))
    model.add(Dropout(0.5))
    model.add(Dense(50, activation='tanh', kernel_initializer='glorot_uniform'))
    model.add(Dense(1, activation='sigmoid'))
    model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
    return model


# Hit_Predictor
def init_model_rnn(shape=None):
    inputs = Input(shape=(None,2))
    h1 = Bidirectional(LSTM(200, return_sequences=True))(inputs)
    h2 = Bidirectional(LSTM(100, return_sequences=False))(h1)
    h3 = RepeatVector(1)(h2)
    outputs = TimeDistributed(Dense(2))(h3)
    model = Model(inputs=inputs, outputs=outputs)
    model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
    return model
