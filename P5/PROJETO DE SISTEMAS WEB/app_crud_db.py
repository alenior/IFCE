from flask import Flask, jsonify, request, abort
from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///mydb.db'
db = SQLAlchemy(app)


class Item(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(80), unique=True, nullable=False)

    def __repr__(self):
        return '<Item %r>' % self.name

    def to_dict(self):
        return {
            'id': self.id,
            'name': self.name
        }    

# Mock database
items = []

@app.route('/items', methods=['POST'])
def create_item():
    if not request.json or 'name' not in request.json:
        abort(400)  # Bad request
    item = {
        'name': request.json['name']
    }
    item  = Item(name=request.json['name'])

    db.session.add(item)
    db.session.commit()
    return jsonify({'item': item.to_dict()}), 201

@app.route('/items', methods=['GET'])
def get_items():
    items = Item.query.all()  # Retrieve all items from the database
    items_list = [item.to_dict() for item in items]  # Convert each item to a dictionary
    # result = []
    # for item in items:
    #     result  = result.append(item.to_dict())
    return jsonify({'items': items_list}), 200  # Return the items as JSON

@app.route('/items/<int:item_id>', methods=['GET'])
def get_item(item_id):
    item = Item.query.get(item_id)  # Retrieve the item by its ID
    if item is None:
        return jsonify({'error': 'Item not found'}), 404  # Return 404 if not found
    return jsonify({'item': item.to_dict()}), 200  # Return the item as JSON

@app.route('/items/<int:item_id>', methods=['PUT'])
def update_item(item_id):
    item = Item.query.get(id)
    if item is None:
        return jsonify({'error': 'Item not found'}), 404  # Return 404 if item doesn't exist

    if not request.json or 'name' not in request.json:
        return jsonify({'error': 'Bad request, "name" field is required'}), 400

    item.name = request.json.get('name')  # Update the item's name
    db.session.commit()  # Save changes to the database

    return jsonify({'item': item.to_dict()}), 200  # Return the updated item as JSON

@app.route('/items/<int:item_id>', methods=['DELETE'])
def delete_item(item_id):
    item = Item.query.get(id)
    if item is None:
        return jsonify({'error': 'Item not found'}), 404  # Return 404 if the item doesn't exist

    db.session.delete(item)  # Delete the item from the database
    db.session.commit()  # Commit the changes


if __name__ == '__main__':
    with app.app_context():
        db.create_all()  # Creates the tables in the database
    app.run(debug=True)