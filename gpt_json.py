import json

filename = "/mnt/e/University/Year4/gpt/conversations.json"
conversations = {
    '4bef19e4-631d-43bb-8807-1e5c3b51f31e',
    'c2e0c06c-72fd-47fb-ae41-b9e195e11ba9',
    'bdd8a087-5dd4-4e64-855e-2467877ddec8',
    '5a913f2b-ce2e-4357-861e-32a969ff4bce',
    '7f584ee4-07a9-4e74-825d-5e6195b8ed8e',
}

if __name__ == "__main__":
    with open(filename) as f:
        data = json.load(f)
    new_json = {}

    for conv_idx in range(len(data)):
        conv = data[conv_idx]
        if (conv['id'] not in conversations):
            continue

        mapping = conv['mapping']
        root = None

        print(conv['title'], conv['id'])

        for key in list(mapping.keys()):
            if mapping[key]['parent'] is None:
                root = key
                break

        queue = [root]

        curr_question = None
        while len(queue) > 0:
            key = queue.pop(0)
            queue.extend(mapping[key]['children'])
            if len(mapping[key]['children']) > 1:
                print("ERROR: Number of Answers Exceeds Expected")

            message = mapping[key]['message']
            if message is None:
                continue

            if message['author']['role'] == 'user':
                curr_question = message['content']['parts']
                if len(message['content']['parts']) > 1:
                    print("ERROR: Number of answe parts exceeds expected")

            if message['author']['role'] != 'user':

                curr_answer = message['content']['parts'][0]

                print("Question: ", curr_question)
                print("Answer: ", curr_answer)
                enter = int(input("Enter 1 for enter and 0 otherwise: "))

                if not enter:
                    continue

                if mapping[key]['parent'] not in new_json:
                    new_json[mapping[key]['parent']] = {}

                feature = input("Enter corresponding feature: ")
                method = "chatgpt"
                file = input("Enter filename")
                line_start = int(input("Approx line start"))
                line_end = int(input("Approx line end"))

                new_json[mapping[key]['parent']]['feature'] = feature
                new_json[mapping[key]['parent']]['method'] = method
                new_json[mapping[key]['parent']]['query'] = curr_question
                new_json[mapping[key]['parent']
                         ]['response'] = message['content']['parts'][0]
                new_json[mapping[key]['parent']]['code'] = (file + ":" +
                                                            str(line_start) + file + ":" + str(line_end))
                if len(message['content']['parts']) > 1:
                    print("ERROR: Number of answe parts exceeds expected")

                print(new_json)
