# Cheat Sheet

## Passing Structs to Function

    struct sampleData {
        int N;
        int M;
        string sample_name;
        string speaker;
    };

    bool data(sampleData *samples)
    {
        samples->N = 10;
        samples->M = 20;
        // etc.
    }

    int main(int argc, char *argv[]) {
        sampleData samples;
        data(&samples);
    }

## 