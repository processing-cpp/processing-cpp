/**
 * Loading JSON Data
 * Translated to C++ Mode from Processing Java by Daniel Shiffman.
 */

struct Bubble {
    float x, y, diameter;
    std::string name;
    bool over = false;

    Bubble(float x, float y, float diameter, std::string name)
        : x(x), y(y), diameter(diameter), name(name) {}

    void rollover(float px, float py) {
        over = dist(px, py, x, y) < diameter / 2;
    }

    void display() {
        stroke(0);
        strokeWeight(2);
        noFill();
        ellipse(x, y, diameter, diameter);
        if (over) {
            fill(0);
            textAlign(CENTER);
            text(name, x, y + diameter / 2 + 20);
        }
    }
};

std::vector<Bubble> bubbles;
JSONObject json;

void loadData() {
    json = loadJSONObject("data/data.json");
    JSONArray bubbleData = json.getJSONArray("bubbles");
    bubbles.clear();

    for (int i = 0; i < bubbleData.size(); i++) {
        JSONObject bubble = bubbleData.getJSONObject(i);
        JSONObject position = bubble.getJSONObject("position");
        float x = position.getFloat("x");
        float y = position.getFloat("y");
        float diameter = bubble.getFloat("diameter");
        std::string label = bubble.getString("label");
        bubbles.push_back(Bubble(x, y, diameter, label));
    }
}

void setup() {
    size(640, 360);
    loadData();
}

void draw() {
    background(255);
    for (auto& b : bubbles) {
        b.display();
        b.rollover(mouseX, mouseY);
    }
    textAlign(LEFT);
    fill(0);
    text("Click to add bubbles.", 10, height - 10);
}

void mousePressed() {
    JSONObject newBubble;
    JSONObject position;
    position.setInt("x", mouseX);
    position.setInt("y", mouseY);
    newBubble.setJSONObject("position", position);
    newBubble.setFloat("diameter", random(40, 80));
    newBubble.setString("label", "New label");

    JSONArray bubbleData = json.getJSONArray("bubbles");
    bubbleData.append(newBubble);
    if (bubbleData.size() > 10) {
        bubbleData.remove(0);
    }
    json.setJSONArray("bubbles", bubbleData);

    saveJSONObject("data/data.json", json);
    loadData();
}
